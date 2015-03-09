#include <string>
#include <gtest/gtest.h>

#include "fileset.h"

using std::string;
using LibprojManager::Interface::FileSetFactory;
using LibprojManager::Interface::FileSetLoader;

class FileSetTest : public ::testing::Test {

protected:
    string projectNormalFile,
           projectEmptyFile,
           projectInconsistentFile,
           projectBrokenFile;
           FileSetLoader * loader = nullptr;

public:
    FileSetTest() { }
    virtual ~FileSetTest() { }

    virtual void SetUp() {
        projectNormalFile = {"project_files/normal.libproject"};
        projectEmptyFile  = {"project_files/empty.libproject"};
        projectInconsistentFile = {"project_files/inconsistent.libproject"};
        projectBrokenFile = {"project_files/empty.libproject"};
    }
    virtual void TearDown() {
        delete loader;
    }

};


/* TESTOPENFILE */
TEST_F(FileSetTest, testOpenFile__NormalFile) {

    if(loader)
        delete loader;
    loader = FileSetFactory::createFileSet(projectNormalFile);
    EXPECT_EQ(1, loader->open()) << "Test for open normal file";

}

TEST_F(FileSetTest, testOpenFile__EmptyFile) {

    if(loader)
        delete loader;
    loader = FileSetFactory::createFileSet(projectEmptyFile);
    EXPECT_EQ(1, loader->open()) << "Test for open empty file";

}

TEST_F(FileSetTest, testOpenFile__InconsistentFile) {

    if(loader)
        delete loader;
    loader = FileSetFactory::createFileSet(projectInconsistentFile);
    EXPECT_EQ(1, loader->open()) << "Test for open Inconsistent file";

}

TEST_F(FileSetTest, testOpenFile__BrokenFile) {

    if(loader)
        delete loader;
    loader = FileSetFactory::createFileSet(projectBrokenFile);
    EXPECT_EQ(1, loader->open()) << "Test for open Broken file";

}

TEST_F(FileSetTest, testGetFileNames__NormalFile) {
    if(loader)
        delete loader;
    loader = FileSetFactory::createFileSet(projectNormalFile);
    ASSERT_NE(0, loader->getFileNames().empty());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

   return RUN_ALL_TESTS();
}
