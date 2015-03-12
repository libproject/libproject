#include <string>
#include <gtest/gtest.h>
#include "fileset.h"

using std::string;
using LibprojManager::Interface::FileSetFactory;
using LibprojManager::Interface::FileSetLoader;

class FileSetTest : public ::testing::Test {

protected:
           FileSetLoader * loader = nullptr;

public:
    FileSetTest() { }
    virtual ~FileSetTest() { }

    virtual void SetUp() {
        if(loader)
            delete loader;
    }

    virtual void TearDown() {
        delete loader;
    }

};

class FileSetTest_NORMALENV : public FileSetTest {
protected:
    string projectNormalFile;

    void SetUp() {
        FileSetTest::SetUp();
        projectNormalFile = {"project_files/normal.libproject"};
    }
};

class FileSetTest_ABNORMALENV : public FileSetTest {
protected:
    string projectEmptyFile,
        projectWithNullProjectKeyFile,
        projectWithNullFilesKeyFile,
        projectNotRelevantFile,
        projectInconsistentFile,
        projectNoFilesArray_DigitFile,
        projectNoFilesArray_StringFile,
        projectBrokenFile;

    void SetUp() {
        FileSetTest::SetUp();
        projectEmptyFile  = {"project_files/empty.libproject"};
        projectWithNullProjectKeyFile  = {"project_files/withnullprojectkey.libproject"};
        projectWithNullFilesKeyFile  = {"project_files/withnullfileskey.libproject"};
        projectNotRelevantFile = {"project_files/notrelevant.libproject"};
        projectNoFilesArray_DigitFile = {"project_files/nofilesarray_digit.libproject"};
        projectNoFilesArray_StringFile = {"project_files/nofilesarray_string.libproject"};
        projectInconsistentFile = {"project_files/inconsistent.libproject"};
        projectBrokenFile = {"project_files/empty.libproject"};
    }
};

/* TESTOPENFILE */
TEST_F(FileSetTest_NORMALENV, testOpenFile) {
    loader = FileSetFactory::createFileSet(projectNormalFile);
    EXPECT_EQ(1, loader->open());

}

TEST_F(FileSetTest_ABNORMALENV, testOpenFile__EmptyFile) {
    loader = FileSetFactory::createFileSet(projectEmptyFile);
    ASSERT_EQ(0, loader->open());

}

TEST_F(FileSetTest_ABNORMALENV, testOpenFile__WithNullProjectKeyFile) {
    loader = FileSetFactory::createFileSet(projectWithNullProjectKeyFile);
    ASSERT_EQ(0, loader->open());

}

TEST_F(FileSetTest_ABNORMALENV, testOpenFile__WithNullFilesKeyFile) {
    loader = FileSetFactory::createFileSet(projectWithNullFilesKeyFile);
    ASSERT_EQ(0, loader->open());

}

TEST_F(FileSetTest_ABNORMALENV, testOpenFile__NotRelevantFile) {
    loader = FileSetFactory::createFileSet(projectNotRelevantFile);
    ASSERT_EQ(0, loader->open());

}

TEST_F(FileSetTest_ABNORMALENV, testOpenFile__InconsistentFile) {
    loader = FileSetFactory::createFileSet(projectInconsistentFile);
    ASSERT_EQ(0, loader->open());

}

TEST_F(FileSetTest_ABNORMALENV, testOpenFile__NoFilesArray_DigitFile) {
    loader = FileSetFactory::createFileSet(projectNoFilesArray_DigitFile);
    ASSERT_EQ(0, loader->open());

}

TEST_F(FileSetTest_ABNORMALENV, testOpenFile__NoFilesArray_StringFile) {
    loader = FileSetFactory::createFileSet(projectNoFilesArray_StringFile);
    ASSERT_EQ(0, loader->open());

}

TEST_F(FileSetTest_ABNORMALENV, testOpenFile__BrokenFile) {
    loader = FileSetFactory::createFileSet(projectBrokenFile);
    ASSERT_EQ(0, loader->open());

}

//
TEST_F(FileSetTest_NORMALENV, testGetFileNames) {
    loader = FileSetFactory::createFileSet(projectNormalFile);
    loader->open();
    ASSERT_LT(0, loader->getFileNames().size());
}

//
TEST_F(FileSetTest_NORMALENV, testGetProjectName) {
    loader = FileSetFactory::createFileSet(projectNormalFile);
    loader->open();
    ASSERT_LT(0, loader->getProjectName().length());
}

//
TEST_F(FileSetTest_NORMALENV, testGetPathToRootNode) {
    loader = FileSetFactory::createFileSet(projectNormalFile);
    loader->open();
    ASSERT_STRNE("", loader->getPathToRootNode().c_str());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

   return RUN_ALL_TESTS();
}
