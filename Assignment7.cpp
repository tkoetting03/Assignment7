/***************************************************************
 * File: Assignment7.cpp
 * Course: EECS 348 - Software Engineering I
 * Assignment: No. 7
 *
 * Description:
 *   This program connects to the EECS MySQL server, creates
 *   copies of the Course, Student, Faculty, Offering, and
 *   Enrollment tables with a "7" suffix, and executes the
 *   twelve SQL queries specified in the handout.
 *   For each query, the program prints the query number, the
 *   English description of the query, and the results in a
 *   simple table-like format on the console.
 *
 * Author:
 *   Thomas Koetting
 *
 * Other Sources:
 *   - ChatGPT (OpenAI) caught bugs and added pointer/memory optimization
 *   - Reddit
 *   - Stackoverflow 
 *
 * Creation Date:
 *   11-18-25
 *
 ***************************************************************/

#include <iostream> 
#include <string> 
#include <stdexcept>
#include <mysql_driver.h>
#include <mysql_connection.h>    
#include <cppconn/statement.h> 
#include <cppconn/resultset.h>   
#include <cppconn/exception.h> 

int main() {

    sql::mysql::MySQL_Driver *driver = nullptr; // pointer to MySQL driver instance
    sql::Connection *con = nullptr; // pointer to MySQL connection
    sql::Statement *stmt = nullptr; // pointer to SQL statement object
    sql::ResultSet *res = nullptr; // pointer to SQL result set object

    std::string username = "348f25_t211k568";
    std::string password = "F@~G?jH8>YiR28!";
    std::string schema  = "348f25_t211k568";

    try {

        driver = sql::mysql::get_mysql_driver_instance(); // get the MYSQL drive instance

        con = driver->connect("tcp://mysql.eecs.ku.edu:3306", username, password); // connect to EECS server

        con->setSchema(schema); // select schema

        stmt = con->createStatement(); // makes a statement object

        stmt->execute("DROP TABLE IF EXISTS Enrollment7"); // drop Enrollment7 if it exists
        stmt->execute("DROP TABLE IF EXISTS Offering7"); // drop Offering7 if it exists
        stmt->execute("DROP TABLE IF EXISTS Student7"); // drop Student7 if it exists
        stmt->execute("DROP TABLE IF EXISTS Faculty7"); // drop Faculty7 if it exists
        stmt->execute("DROP TABLE IF EXISTS Course7"); // drop Course7 if it exists

        stmt->execute("CREATE TABLE Course7 LIKE Course"); // copy table structure
        stmt->execute("INSERT INTO Course7 SELECT * FROM Course"); // copy table data

        stmt->execute("CREATE TABLE Student7 LIKE Student"); // copy table structure
        stmt->execute("INSERT INTO Student7 SELECT * FROM Student"); // copy table data

        stmt->execute("CREATE TABLE Faculty7 LIKE Faculty"); // copy table structure
        stmt->execute("INSERT INTO Faculty7 SELECT * FROM Faculty"); // copy table data

        stmt->execute("CREATE TABLE Offering7 LIKE Offering"); // copy table structure
        stmt->execute("INSERT INTO Offering7 SELECT * FROM Offering"); // copy table data

        stmt->execute("CREATE TABLE Enrollment7 LIKE Enrollment"); // copy table structure
        stmt->execute("INSERT INTO Enrollment7 SELECT * FROM Enrollment"); // copy table data

        std::cout << std::endl; // print endline

        // query 1
        std::cout << "1. Retrieve all student information for those whose major is 'IS' from the Student table." << std::endl;

        res = stmt->executeQuery( // execute query on Student7
            "SELECT * FROM Student7 WHERE StdMajor = 'IS';"
        );

        std::cout << "StdNo\tStdFirstName\tStdLastName\tStdCity\tStdState\tStdZip\tStdMajor\tStdClass\tStdGPA" << std::endl; // print row for the student columns

        // loop through each row in the result set
        while (res->next()) { // move to next row; returns false when no more rows
            std::cout << res->getString("StdNo")        << '\t' 
                      << res->getString("StdFirstName") << '\t' 
                      << res->getString("StdLastName")  << '\t' 
                      << res->getString("StdCity")      << '\t' 
                      << res->getString("StdState")     << '\t' 
                      << res->getString("StdZip")       << '\t' 
                      << res->getString("StdMajor")     << '\t' 
                      << res->getString("StdClass")     << '\t' 
                      << res->getDouble("StdGPA")       << std::endl; 
        }

        delete res; // reset pointer
        res = nullptr; // get rid of dangling pointer
        std::cout << std::endl; // blank line

        // query 2
        std::cout << "2. Retrieve the student numbers of students who have enrolled in more than two courses from the Enrollment table." << std::endl;

        res = stmt->executeQuery( // Execute query 2 on Enrollment7
            "SELECT StdNo "
            "FROM Enrollment7 "
            "GROUP BY StdNo "
            "HAVING COUNT(*) > 2;"
        );

        std::cout << "StdNo" << std::endl; // print header for StdNo column

        while (res->next()) { // loop through the result set rows
            std::cout << res->getString("StdNo") << std::endl; // print student number
        }

        delete res; // free memory
        res = nullptr; // get rid of dangling pointer
        std::cout << std::endl; // blank line

        // query 3
        std::cout << "3. List the first and last names of all professors from the Faculty table who have been employed for more than 15 years as of 2025, including those hired in 2010 or earlier." << std::endl;

        res = stmt->executeQuery( // execute query 3 on Faculty7
            "SELECT FacFirstName, FacLastName "
            "FROM Faculty7 "
            "WHERE FacHireDate <= '2010-12-31';"
        );

        std::cout << "FacFirstName\tFacLastName" << std::endl;

        while (res->next()) { // loop through each faculty row
            std::cout << res->getString("FacFirstName") << '\t' // print first name
                      << res->getString("FacLastName")  << std::endl; // print last name
        }

        delete res; // delete result set
        res = nullptr; // reset pointer
        std::cout << std::endl; // blank line

        // query 4
        std::cout << "4. Retrieve all course numbers from the Offering table that were offered in Summer 2020." << std::endl;

        res = stmt->executeQuery( // execute query 4 on Offering7
            "SELECT CourseNo "
            "FROM Offering7 "
            "WHERE OffTerm = 'SUMMER' AND OffYear = 2020;"
        );

        std::cout << "CourseNo" << std::endl; // print header for CourseNo

        while (res->next()) { // loop through course number results
            std::cout << res->getString("CourseNo") << std::endl; // print course number
        }

        delete res; // free result set
        res = nullptr;   // reset pointer
        std::cout << std::endl; // blank line

        // query 5
         std::cout << "5. List the first and last names of all professors from the Faculty table who live in ZIP code 98114." << std::endl;

        res = stmt->executeQuery( // execute query 5 on Faculty7
            "SELECT FacFirstName, FacLastName "
            "FROM Faculty7 "
            "WHERE FacZipCode LIKE '98114%';"
        );

        std::cout << "FacFirstName\tFacLastName" << std::endl; // print header for faculty names

        while (res->next()) { // loop through faculty rows
            std::cout << res->getString("FacFirstName") << '\t' // print first name
                      << res->getString("FacLastName")  << std::endl; // print last name
        }

        delete res; // delete result set
        res = nullptr; // reset pointer
        std::cout << std::endl; // blank line

        // query 6
        std::cout << "6. Retrieve the second-highest GPA from the Student table." << std::endl;

        res = stmt->executeQuery( // execute query 6 on Student7
            "SELECT MAX(StdGPA) AS SecondHighestGPA "
            "FROM Student7 "
            "WHERE StdGPA < (SELECT MAX(StdGPA) FROM Student7);"
        );

        std::cout << "SecondHighestGPA" << std::endl; // print header for GPA
        
        while (res->next()) { // loop through each GPA row
            std::cout << res->getDouble("SecondHighestGPA") << std::endl; // print second highest GPA
        }

        delete res; // free result set
        res = nullptr; // reset pointer
        std::cout << std::endl; // blank line

        // query 7
        std::cout << "7. Retrieve the first and last names of individuals who appear in both the Student and Faculty tables." << std::endl;

        res = stmt->executeQuery( // execute query 7 on first and last name between Student7 and Faculty7
            "SELECT DISTINCT s.StdFirstName, s.StdLastName "
            "FROM Student7 AS s "
            "JOIN Faculty7 AS f "
            "ON s.StdFirstName = f.FacFirstName "
            "AND s.StdLastName = f.FacLastName;"
        );

        
        std::cout << "FirstName\tLastName" << std::endl; // print header for overlapping names

        
        while (res->next()) { // loop through matched rows
            std::cout << res->getString("StdFirstName") << '\t' // print first name
                      << res->getString("StdLastName")  << std::endl; // print last name
        }

        delete res; // free result set
        res = nullptr; // reset pointer
        std::cout << std::endl; // blank line

        // query 8
        std::cout << "8. Retrieve the student number, first and last names, and the number of selected courses from the Student and Enrollment tables." << std::endl;

        res = stmt->executeQuery( // execute SQL query 8
            "SELECT s.StdNo, s.StdFirstName, s.StdLastName, "
            "       COUNT(e.OfferNo) AS NumCourses "
            "FROM Student7 AS s "
            "LEFT JOIN Enrollment7 AS e ON s.StdNo = e.StdNo "
            "GROUP BY s.StdNo, s.StdFirstName, s.StdLastName "
            "ORDER BY s.StdNo;"
        );

        std::cout << "StdNo\tStdFirstName\tStdLastName\tNumCourses" << std::endl; // print header for student and course count

        while (res->next()) { // loop through rows, printing each student's info and number of courses
            std::cout << res->getString("StdNo")        << '\t' // student number
                      << res->getString("StdFirstName") << '\t' // first name
                      << res->getString("StdLastName")  << '\t' // last name
                      << res->getInt("NumCourses")      << std::endl; // number of courses
        }

        delete res; // free result set
        res = nullptr; // reset pointer
        std::cout << std::endl; // blank line

        // query 9
        std::cout << "9. Retrieve the first and last names of professors, along with their salaries, for the top three highest-paid professors from the Faculty table." << std::endl;

        res = stmt->executeQuery( // execute SQL query 9 on Faculty7
            "SELECT FacFirstName, FacLastName, FacSalary "
            "FROM Faculty7 "
            "ORDER BY FacSalary DESC "
            "LIMIT 3;"
        );

        std::cout << "FacFirstName\tFacLastName\tFacSalary" << std::endl; // print header for faculty names and salaries

        while (res->next()) { // loop through the top three highest-paid professors
            std::cout << res->getString("FacFirstName") << '\t' // print first name
                      << res->getString("FacLastName")  << '\t' // print last name
                      << res->getDouble("FacSalary")    << std::endl; // print salary
        }

        delete res; // free result set
        res = nullptr;   // reset pointer
        std::cout << std::endl; // blank line

        // query 10
        std::cout << "10. Retrieve all student information from the Student table for students who do not have any enrollment records in the Enrollment table." << std::endl;

        res = stmt->executeQuery( // execute SQL query 10 on Enrollment7
            "SELECT * "
            "FROM Student7 AS s "
            "WHERE NOT EXISTS ( "
            "    SELECT 1 "
            "    FROM Enrollment7 AS e "
            "    WHERE e.StdNo = s.StdNo "
            ");"
        );

        std::cout << "StdNo\tStdFirstName\tStdLastName\tStdCity\tStdState\tStdZip\tStdMajor\tStdClass\tStdGPA" << std::endl; // print header for full student information

        
        while (res->next()) { // loop through students with no enrollment records
            std::cout << res->getString("StdNo")        << '\t' // student number
                      << res->getString("StdFirstName") << '\t' // first name
                      << res->getString("StdLastName")  << '\t' // last name
                      << res->getString("StdCity")      << '\t' // city
                      << res->getString("StdState")     << '\t' // state
                      << res->getString("StdZip")       << '\t' // ZIP
                      << res->getString("StdMajor")     << '\t' // major
                      << res->getString("StdClass")     << '\t' // class
                      << res->getDouble("StdGPA")       << std::endl; // GPA
        }

        delete res; // free result set
        res = nullptr; // reset pointer
        std::cout << std::endl; // blank line

        // query 11
        std::cout << "11. Insert a junior CS student named Alice Smith from Topeka, Kansas (ZIP 66610), with student number 888-88-8888 and a GPA of 3.85 into the Student table. Then display the updated table." << std::endl;

        stmt->execute( // execute SQL INSERT for new student Alice Smith into Student7
            "INSERT INTO Student7 "
            "(StdNo, StdFirstName, StdLastName, StdCity, StdState, StdZip, StdMajor, StdClass, StdGPA) "
            "VALUES ('888-88-8888', 'ALICE', 'SMITH', 'TOPEKA', 'KS', '66610', 'CS', 'JR', 3.85);"
        );

        res = stmt->executeQuery("SELECT * FROM Student7;"); // now select and display all rows from Student7

        std::cout << "StdNo\tStdFirstName\tStdLastName\tStdCity\tStdState\tStdZip\tStdMajor\tStdClass\tStdGPA" << std::endl; // print header for the updated student table

        while (res->next()) { // loop through and print every student row
            std::cout << res->getString("StdNo")        << '\t' // student number
                      << res->getString("StdFirstName") << '\t' // first name
                      << res->getString("StdLastName")  << '\t' // last name
                      << res->getString("StdCity")      << '\t' // city
                      << res->getString("StdState")     << '\t' // state
                      << res->getString("StdZip")       << '\t' // ZIP
                      << res->getString("StdMajor")     << '\t' // major
                      << res->getString("StdClass")     << '\t' // class
                      << res->getDouble("StdGPA")       << std::endl; // GPA
        }

        delete res; // free result set
        res = nullptr; // reset pointer
        std::cout << std::endl; // blank line

        // query 12
        std::cout << "12. Student Bob Norbert has moved to Overland Park, Kansas. Update the StdCity and StdZip fields in the Student table to reflect this change, and then display the updated table." << std::endl;

        stmt->execute( // update Bob Norbert's city, state, and ZIP in Student7
            "UPDATE Student7 "
            "SET StdCity = 'OVERLAND PARK', "
            "    StdState = 'KS', "
            "    StdZip = '66210' "
            "WHERE StdNo = '124-56-7890';"
        );

        res = stmt->executeQuery("SELECT * FROM Student7;"); // select all rows to show the updated table

        std::cout << "StdNo\tStdFirstName\tStdLastName\tStdCity\tStdState\tStdZip\tStdMajor\tStdClass\tStdGPA" << std::endl; // print header for updated Student7 table

        while (res->next()) { // loop through and print every student row
            std::cout << res->getString("StdNo")        << '\t' // student number
                      << res->getString("StdFirstName") << '\t' // first name
                      << res->getString("StdLastName")  << '\t' // last name
                      << res->getString("StdCity")      << '\t' // city
                      << res->getString("StdState")     << '\t' // state
                      << res->getString("StdZip")       << '\t' // ZIP
                      << res->getString("StdMajor")     << '\t' // major
                      << res->getString("StdClass")     << '\t' // class
                      << res->getDouble("StdGPA")       << std::endl; // GPA
        }

        delete res; // Free final result set
        res = nullptr; // reset pointer

        std::cout << std::endl;
        std::cout << "All 12 queries executed successfully." << std::endl;  // print final message

        delete stmt; // delete statement object
        stmt = nullptr; // reset pointer
        delete con; // delete connection object
        con = nullptr; // reset pointer
        }

    catch (sql::SQLException &e) { // Catch MySQL-specific exceptions
        std::cerr << "SQL error: " << e.what() << std::endl; // Print SQL error message
    }
    catch (std::exception &e) {    // Catch standard library exceptions
        std::cerr << "Standard exception: " << e.what() << std::endl; // Print error message
    }
    catch (...) { // Catch any other unexpected exceptions
        std::cerr << "Unknown exception occurred." << std::endl; // Generic error message
    }

    // --------- Final clean-up in case an exception occurred before deletion ---------
    if (res != nullptr) { // If result set pointer is not null
        delete res;       // Delete result set object
        res = nullptr;    // Reset pointer
    }

    if (stmt != nullptr) { // If statement pointer is not null
        delete stmt;       // Delete statement object
        stmt = nullptr;    // Reset pointer
    }

    if (con != nullptr) {  // If connection pointer is not null
        delete con;        // Delete connection object
        con = nullptr;     // Reset pointer
    }
    return 0;
}
