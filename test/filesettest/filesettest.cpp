#include <string>
#include <list>
#include <gtest/gtest.h>
#include "fileset.h"
#include <algorithm>
#include <iterator>

using std::string;
using std::list;
using LibprojManager::Interface::FileSetFactory;
using LibprojManager::Interface::FileSetLoader;

class FileSetTest : public ::testing::Test {

protected:
  FileSetLoader *loader = nullptr;

public:
  FileSetTest() {}
  virtual ~FileSetTest() {}

  virtual void SetUp() {
    if (loader)
      delete loader;
  }

  virtual void TearDown() { delete loader; }
};

class FileSetTest_NORMALENV : public FileSetTest {
protected:
  string projectNormalFile;
  string projectNameRef;
  list<string> projectFilesRef;

  void SetUp() {
    FileSetTest::SetUp();
    projectNormalFile = {"project_files/normal.libproject"};
    projectNameRef = {"SameAmmoniteOnPurpleSkies *** 42"};
    projectFilesRef = list<string>{
        "main.cpp", "Test.h", "Test.cpp", "Foo.h", "Foo.cpp", "README",
        "kekeke.cpp", "testnew.cpp", "testadd2.cpp", "testadd3.cpp"};
  }
};

class FileSetTest_ABNORMALENV : public FileSetTest {
protected:
  string projectEmptyFile, projectWithNullProjectKeyFile,
      projectWithNullFilesKeyFile, projectNotRelevantFile,
      projectInconsistentFile, projectNoFilesArray_DigitFile,
      projectNoFilesArray_StringFile, projectBrokenFile;

  void SetUp() {
    FileSetTest::SetUp();
    projectEmptyFile = {"project_files/empty.libproject"};
    projectWithNullProjectKeyFile = {
        "project_files/withnullprojectkey.libproject"};
    projectWithNullFilesKeyFile = {"project_files/withnullfileskey.libproject"};
    projectNotRelevantFile = {"project_files/notrelevant.libproject"};
    projectNoFilesArray_DigitFile = {
        "project_files/nofilesarray_digit.libproject"};
    projectNoFilesArray_StringFile = {
        "project_files/nofilesarray_string.libproject"};
    projectInconsistentFile = {"project_files/inconsistent.libproject"};
    projectBrokenFile = {"project_files/empty.libproject"};
  }
};

TEST_F(FileSetTest_NORMALENV, testOpenFile) {
  loader = FileSetFactory::createFileSet(projectNormalFile);
  EXPECT_EQ(true, loader->open());
}

TEST_F(FileSetTest_NORMALENV, testGetPathToRootNode) {
  loader = FileSetFactory::createFileSet(projectNormalFile);
  loader->open();
  ASSERT_NE(string(""), loader->getPathToRootNode());
}

TEST_F(FileSetTest_NORMALENV, checkProjectName) {
  loader = FileSetFactory::createFileSet(projectNormalFile);
  loader->open();
  EXPECT_EQ(projectNameRef, loader->getProjectName());
}

TEST_F(FileSetTest_NORMALENV, checkFileList) {
  loader = FileSetFactory::createFileSet(projectNormalFile);
  loader->open();
  list<string> testingList(loader->getFileNames()),
               diff;
  std::front_insert_iterator<list<string> > fIter(diff);
  std::set_symmetric_difference(
      projectFilesRef.cbegin(), projectFilesRef.cend(),
      testingList.cbegin(), testingList.cend(), fIter);
  EXPECT_EQ(0, diff.size());
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

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
