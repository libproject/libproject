#include <string>
#include <gtest/gtest.h>

#include "fileset.h"

using std::string;
using LibprojManager::Interface::FileSetFactory;
using LibprojManager::Interface::FileSetLoader;

class FileSetTest : public ::testing::Test {
    //FileSetTest() { }
    //virtual ~FileSetTest() { }

    virtual void SetUp() { }
    virtual void TearDown() { }

};


TEST_F(FileSetTest, APIOpenNormalFile) {
    const string projectFileName =
            "project_files/normal.libproject";
    FileSetLoader * loader =
            FileSetFactory::createFileSet(projectFileName);
    EXPECT_EQ(1, loader->open()) << "Test for open normal file";

}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

   return RUN_ALL_TESTS();
}
