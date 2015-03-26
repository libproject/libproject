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
    "project_files/notexist.libproject"};
}

namespace FileSetTests {
class TestSkeleton : public ::testing::Test {

protected:
  FileSetLoader *loader = nullptr;

public:
  TestSkeleton() {}
  virtual ~TestSkeleton() {}

  virtual void SetUp() {
    if (loader)
      delete loader;
  }

  virtual void TearDown() { delete loader; }
};

class TestRegularInput : public TestSkeleton {
protected:
  string Normal;
  string projectNameRef;
  list<string> projectFilesRef;

  void SetUp() {
    TestSkeleton::SetUp();
    Normal = {"project_files/normal.libproject"};
    projectNameRef = {"SameAmmoniteOnPurpleSkies *** 42"};
    projectFilesRef = {"main.cpp",    "Test.h",      "Test.cpp",
                       "Foo.h",       "Foo.cpp",     "README",
                       "kekeke.cpp",  "testnew.cpp", "testadd2.cpp",
                       "testadd3.cpp"};
  }
};

class TestAbnormalInput : public TestSkeleton,
                          public ::testing::WithParamInterface<string> {
protected:
  void SetUp() { TestSkeleton::SetUp(); }
  void TearDown() {}
};

TEST_F(TestRegularInput, Open_file) {
  loader = FileSetFactory::createFileSet(Normal);
  ASSERT_NO_THROW(loader->open());
}

TEST_F(TestRegularInput, Open_file_while_already_loaded) {
  loader = FileSetFactory::createFileSet(Normal);
  loader->open();
  ASSERT_THROW(loader->open(), FileSetRuntimeError);
}

INSTANTIATE_TEST_CASE_P(InstantiationOfTestAbnormalInput, TestAbnormalInput,
                        ::testing::ValuesIn(pathsToAbnormalSources));
TEST_P(TestAbnormalInput, Set_of_attempts_to_open_abnormal_files) {
  ASSERT_THROW({
                 loader = FileSetFactory::createFileSet(GetParam());
                 loader->open();
               },
               FileSetRuntimeError);
}

TEST_F(TestRegularInput, Get_path_to_root_node) {
  loader = FileSetFactory::createFileSet(Normal);
  loader->open();
  ASSERT_NE(string(""), loader->getPathToRootNode());
}

TEST_F(TestRegularInput, Get_path_to_root_node_for_not_loaded) {
  loader = FileSetFactory::createFileSet(Normal);
  ASSERT_THROW(loader->getPathToRootNode(), FileSetRuntimeError);
}

TEST_F(TestRegularInput, Get_project_name) {
  loader = FileSetFactory::createFileSet(Normal);
  loader->open();
  EXPECT_EQ(projectNameRef, loader->getProjectName());
}

TEST_F(TestRegularInput, Get_project_name_for_not_loaded) {
  loader = FileSetFactory::createFileSet(Normal);
  ASSERT_THROW(loader->getProjectName(), FileSetRuntimeError);
}

TEST_F(TestRegularInput, Get_list_of_files) {
  loader = FileSetFactory::createFileSet(Normal);
  loader->open();
  EXPECT_EQ(true, loader->getFileNames() == projectFilesRef);
}

TEST_F(TestRegularInput, Get_list_of_files_for_not_loaded) {
  loader = FileSetFactory::createFileSet(Normal);
  ASSERT_THROW(loader->getFileNames(), FileSetRuntimeError);
}
} // namespace FileSetTests

int main(int argc, char **argv) {
  using namespace FileSetTests;
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
