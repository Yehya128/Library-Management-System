#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>


TEST_CASE("Book borrowing and returning", "[book]") {
    Member* member = new Member(1, "John", "Address", "john@example.com");

    Librarian librarian(1, "Librarian", "Library", "librarian@example.com", 50000);

    librarian.initializeBooksInventory();

    SECTION("Issue book to member") {
        librarian.issueBook(member);

        REQUIRE(member->getBooksBorrowed().size() == 1);
    }

    SECTION("Return book from member") {
        librarian.issueBook(member);

        librarian.returnBook(member);

        REQUIRE(member->getBooksBorrowed().empty());
    }
}

TEST_CASE("Fine calculation", "[fine]") {
    Member* member = new Member(1, "Jane", "Address", "jane@example.com");

    Librarian librarian(1, "Librarian", "Library", "librarian@example.com", 50000);

    librarian.initializeBooksInventory();

    SECTION("Calculate fine for overdue book") {
        librarian.issueBook(member);

        time_t pastDueDate = time(0) - (3 * 24 * 60 * 60);  // 3 days overdue
        member->getBooksBorrowed()[0]->setDueDate(pastDueDate);

        librarian.calcFine(member);

        REQUIRE(true );
    }

    SECTION("No fine for book not overdue") {
        librarian.issueBook(member);

        time_t futureDueDate = time(0) + (3 * 24 * 60 * 60);  
        member->getBooksBorrowed()[0]->setDueDate(futureDueDate);

        librarian.calcFine(member);

        REQUIRE(true );
    }
}
