#include <string>
#include <list>
#include <gtest/gtest.h>
#include "libproject.h"
#include "libproject_error.h"
#include <map>

using std::string;
using std::list;
using LibprojManager::Interface::FileSetFactory;
using LibprojManager::Interface::FileSetLoader;
using namespace LibprojManager::Interface::Error;
using std::map;

namespace {
list<string> pathsToSingleAbnormal = {
    "project_files/single/empty.libproject",
    "project_files/single/notrelevantfileskey.libproject",
    "project_files/single/filesarrayisdigit.libproject",
    "project_files/single/filesarrayisstring.libproject",
    "project_files/single/nofileskey.libproject",
    "project_files/single/broken.libproject",
    "project_files/single/noprojectkey.libproject",
    "project_files/single/notexist.libproject"};
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

class TestRegularSingle : public TestSkeleton {
protected:
  string PathToFile;
  string projectNameRef;
  list<string> projectFilesRef;

  void SetUp() {
    TestSkeleton::SetUp();
    PathToFile = {"project_files/single/normal.libproject"};
    projectNameRef = {"SameAmmoniteOnPurpleSkies *** 42"};
    projectFilesRef = {"main.cpp",    "Test.h",      "Test.cpp",
                       "Foo.h",       "Foo.cpp",     "README",
                       "kekeke.cpp",  "testnew.cpp", "testadd2.cpp",
                       "testadd3.cpp"};
  }
};

class TestAbnormalSingles : public TestSkeleton,
                          public ::testing::WithParamInterface<string> {
protected:
  void SetUp() { TestSkeleton::SetUp(); }
  void TearDown() {}
};

class TestRegularNested : public TestSkeleton {
protected:
  string PathToFile;
  string projectNameRef;
  list<string> projectFilesRef;
  int subprojectsCount;
  list<string> subprojectsNamesRef;
  list<list<string>> subprojectsFilesRef;

  void SetUp() {
    TestSkeleton::SetUp();
    PathToFile = {"project_files/nested/normal.libproject"};
    projectNameRef = {"Nested *** 43"};
    projectFilesRef = {"main.cpp", "Test.h", "Test.cpp"};
    subprojectsCount = 2;
    subprojectsNamesRef = {"subpr1", "subpr2"};
    subprojectsFilesRef = {{"README", "header.h"}, {"INSTALL", "main.cpp"}};
  }
};

TEST_F(TestRegularSingle, Open_file) {
  loader = FileSetFactory::createFileSet(PathToFile);
  ASSERT_NO_THROW(loader->open());
}

TEST_F(TestRegularNested, Open_file) {
  loader = FileSetFactory::createFileSet(PathToFile);
  ASSERT_NO_THROW(loader->open());
}

TEST_F(TestRegularSingle, Open_file_while_already_loaded) {
  loader = FileSetFactory::createFileSet(PathToFile);
  loader->open();
  ASSERT_THROW(loader->open(), FileSetRuntimeError);
}

INSTANTIATE_TEST_CASE_P(InstantiationOfTestAbnormalSingles, TestAbnormalSingles,
                        ::testing::ValuesIn(pathsToSingleAbnormal));
TEST_P(TestAbnormalSingles, Set_of_attempts_to_open_abnormal_files) {
  ASSERT_THROW({
                 loader = FileSetFactory::createFileSet(GetParam());
                 loader->open();
               },
               FileSetRuntimeError);
}

TEST_F(TestRegularSingle, Get_path_to_root_node) {
  loader = FileSetFactory::createFileSet(PathToFile);
  loader->open();
  ASSERT_NO_THROW(loader->getPathToNode());
}

TEST_F(TestRegularNested, Get_path_to_1st_subnode) {
    loader = FileSetFactory::createFileSet(PathToFile);
    loader->open();
    map<string, FileSetLoader *> msl = loader->getSubprojectLoaders();
    //msl[*subprojectsNamesRef.begin()]->open();
    //ASSERT_NO_THROW(msl[*subprojectsNamesRef.begin()]->getPathToNode());
}

TEST_F(TestRegularSingle, Get_path_to_root_node_for_not_loaded) {
  loader = FileSetFactory::createFileSet(PathToFile);
  ASSERT_THROW(loader->getPathToNode(), FileSetRuntimeError);
}

TEST_F(TestRegularSingle, Get_project_name) {
  loader = FileSetFactory::createFileSet(PathToFile);
  loader->open();
  EXPECT_EQ(projectNameRef, loader->getProjectName());
}

TEST_F(TestRegularSingle, Get_project_name_for_not_loaded) {
  loader = FileSetFactory::createFileSet(PathToFile);
  ASSERT_THROW(loader->getProjectName(), FileSetRuntimeError);
}

TEST_F(TestRegularSingle, Get_list_of_files) {
  loader = FileSetFactory::createFileSet(PathToFile);
  loader->open();
  EXPECT_EQ(true, loader->getFileNames() == projectFilesRef);
}

TEST_F(TestRegularSingle, Get_list_of_files_for_not_loaded) {
  loader = FileSetFactory::createFileSet(PathToFile);
  ASSERT_THROW(loader->getFileNames(), FileSetRuntimeError);
}


} // namespace FileSetTests

int main(int argc, char **argv) {
  using namespace FileSetTests;
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
