#include <QTest>
#include "testjsonrepository.h"
#include "testrequestcontext.h"

int main(int argc, char *argv[]) {
    int status = 0;

    TestRequestContext requestTest;
    status |= QTest::qExec(&requestTest, argc, argv);

    TestJsonRepository jsonTest;
    status |= QTest::qExec(&jsonTest, argc, argv);

    return status;
}
