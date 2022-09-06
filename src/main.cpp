/* MIT License
*
* Copyright (c) 2022 Iman Ahmadvand
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#include <iostream>
#include <memory>
#include <boost/uuid/random_generator.hpp>
#include <odb/schema-catalog.hxx>
#include <odb/pgsql/database.hxx>

#include <odb/Data-odb.h>

int main(int argc, char* argv[]) {
    if (argc > 1 && argv[1] == std::string ("--help")) {
        std::cout << "Usage: " << argv[0] << " [options]" << std::endl
                  << "Options:" << std::endl;
        odb::pgsql::database::print_usage(std::cout);
        std::exit(EXIT_SUCCESS);
    }

    std::unique_ptr<odb::database> db{new odb::pgsql::database(argc, argv)};

    try {
        db->schema_version();
    }  catch (std::exception& e) {
        std::cerr << e.what();
    }

    try {
        odb::transaction tr(db->begin());
        odb::schema_catalog::create_schema(*db, "", false);
        tr.commit();
    }  catch (std::exception& e) {
        std::cerr << e.what();
    }

    Student item;
    item.age = 27;
    item.data.SetObject();
    item.gender = Gender::Female;
    item.uid = boost::uuids::random_generator()();
    {
        std::string buffer(reinterpret_cast<char*>(&item.uid.data), reinterpret_cast<char*>(item.uid.data) + sizeof (item.uid.data));
        item.id = std::hash<std::string> {}(buffer);
    }
    item.lastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    item.name = "Student";
    item.score = 12.0;

    // insert
    try {
        odb::transaction tr(db->begin());
        db->persist(item);
        tr.commit();
    }  catch (std::exception& e) {
        std::cerr << e.what();
    }

    // update
    try {
        odb::transaction tr(db->begin());
        item.name = "Foo";
        db->update(item);
        tr.commit();
    }  catch (std::exception& e) {
        std::cerr << e.what();
    }

    // query
    try {
        odb::transaction tr(db->begin());
        auto result = db->query<Student>();
        for (const auto& it : result) {
            std::cout << it << std::endl;
        }
        tr.commit();
    }  catch (std::exception& e) {
        std::cerr << e.what();
    }

    // erase
    try {
        odb::transaction tr(db->begin());
        db->erase(item);
        tr.commit();
    }  catch (std::exception& e) {
        std::cerr << e.what();
    }

    return EXIT_SUCCESS;
}
