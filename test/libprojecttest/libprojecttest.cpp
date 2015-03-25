#include <string>
#include <list>
#include <gtest/gtest.h>
#include "libproject.h"
#include "libproject_error.h"

using std::string;
using std::list;
using LibprojManager::Interface::FileSetFactory;
using LibprojManager::Interface::FileSetLoader;
using namespace LibprojManager::Interface::Error;

namespace {
list<string> pathsToAbnormalSources = {
    "project_files/empty.libproject",
    "project_files/notrelevantfileskey.libproject",
    "project_files/filesarrayisdigit.libproject",
    "project_files/filesarrayisstring.libproject",
    "project_files/nofileskey.libproject",
    "project_files/broken.libproject",
    "project_files/noprojectkey.libproject",
    "project_files/notexist.libproject"
};
}

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

class FileSetTest_NORMALINPUT : public FileSetTest {
protected:
  string Normal;
  string projectNameRef;
  list<string> projectFilesRef;

  void SetUp() {
    FileSetTest::SetUp();
    Normal = {"project_files/normal.libproject"};
    projectNameRef = {"SameAmmoniteOnPurpleSkies *** 42"};
    projectFilesRef = {
        "main.cpp", "Test.h", "Test.cpp", "Foo.h", "Foo.cpp", "README",
        "kekeke.cpp", "testnew.cpp", "testadd2.cpp", "testadd3.cpp"
    };
  }
};

class FileSetTest_ABNORMALINPUT : public FileSetTest,
                                  public ::testing::WithParamInterface<string>
{
protected:
    void SetUp() {
        FileSetTest::SetUp();
    }
    void TearDown() { }
};

INSTANTIATE_TEST_CASE_P(Instantiation_FileSetTest_ABNORMALINPUT,
                        FileSetTest_ABNORMALINPUT,
                        ::testing::ValuesIn(pathsToAbnormalSources));

TEST_F(FileSetTest_NORMALINPUT, testOpenFile) {
  loader = FileSetFactory::createFileSet(Normal);
  ASSERT_NO_THROW(loader->open());
}

TEST_F(FileSetTest_NORMALINPUT, testGetPathToRootNode) {
  loader = FileSetFactory::createFileSet(Normal);
  loader->open();
  ASSERT_NE(string(""), loader->getPathToRootNode());
}

TEST_F(FileSetTest_NORMALINPUT, checkProjectName) {
  loader = FileSetFactory::createFileSet(Normal);
  loader->open();
  EXPECT_EQ(projectNameRef, loader->getProjectName());
}

TEST_F(FileSetTest_NORMALINPUT, checkFileList) {
  loader = FileSetFactory::createFileSet(Normal);
  loader->open();
  EXPECT_EQ(true, loader->getFileNames() == projectFilesRef);
}

TEST_F(FileSetTest_NORMALINPUT, Open_File_While_Already_Opened) {
    loader = FileSetFactory::createFileSet(Normal);
    loader->open();
    ASSERT_THROW(loader->open(), FileSetLogicError);
}

TEST_P(FileSetTest_ABNORMALINPUT, testingSetOfAbnormalInputs) {
    ASSERT_THROW({
                     loader = FileSetFactory::createFileSet(GetParam());
                     loader->open();
                 },
                 FileSetRuntimeError);
}

TEST_F(FileSetTest_NORMALINPUT, Get_Project_Name_For_Not_Loaded_FileSet) {
    loader = FileSetFactory::createFileSet(Normal);
    ASSERT_EQ(string(""), loader->getProjectName());
}

TEST_F(FileSetTest_NORMALINPUT, Get_File_Names_For_Not_Loaded_FileSet) {
    loader = FileSetFactory::createFileSet(Normal);
    ASSERT_EQ(0, loader->getFileNames().size());
}

TEST_F(FileSetTest_NORMALINPUT, Get_Path_To_Root_Node_For_Not_Loaded_FileSet) {
    loader = FileSetFactory::createFileSet(Normal);
    ASSERT_EQ(string(""), loader->getPathToRootNode());
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
