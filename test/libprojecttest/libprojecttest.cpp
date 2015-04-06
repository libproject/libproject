#include <string>
#include <list>
#include <gtest/gtest.h>
#include "libproject.h"
#include "libproject_error.h"
#include <map>
#include <vector>
#include "json.hpp"
#include <fstream>

using std::string;
using std::list;
using LibprojManager::Interface::FileSetFactory;
using LibprojManager::Interface::FileSetLoader;
using namespace LibprojManager::Interface::Error;
using std::map;
using std::vector;
using nlohmann::json;
using std::ifstream;

namespace {
list<string> pathsToSingleAbnormal = {
    "project_files/single/empty.libproject",
    "project_files/single/notrelevantfileskey.libproject",
    "project_files/single/filesarrayisdigit.libproject",
    "project_files/single/filesarrayisstring.libproject",
    "project_files/single/nofileskey.libproject",
    "project_files/single/broken.libproject",
    "project_files/single/noprojectkey.libproject",
    "project_files/single/notexist.libproject"
};
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
  string dirPath;
  string projectNameRef;
  list<string> projectFilesRef;
  int subprojectsCount;
  vector<string> subprojectsFilesRef;
  vector<string> subprojectsNamesRef;

  void SetUp() {
    TestSkeleton::SetUp();
    PathToFile = {"project_files/nested/normal.libproject"};
    dirPath = {"project_files/nested/"};
    projectNameRef = {"Nested *** 43"};
    projectFilesRef = {"main.cpp", "Test.h", "Test.cpp"};
    subprojectsCount = 2;
    subprojectsFilesRef = {"sub/s1.libproject", "sub/s2.libproject"};
    subprojectsNamesRef = {"subpr1", "subpr2"};
  }
};

class TestAddRegularSubprojectsToSingle : public TestSkeleton {
protected:
    string pathToMainFileWhereWillBeSingleSubproject;
    string pathToMainFileWhereWillBeNestedSubproject;
    string pathToMainFileWhichIsTargetForAddBrokenSubproject;
    json contentReferenceWithSingle;
    json contentReferenceWithNested;
    json contentReferenceForTryToAddBroken;
    vector<string> pathToOneRegularSingleSubproject;
    vector<string> pathToOneRegularNestedSubproject;
    vector<string> pathToBrokenSubproject;
    void SetUp() {
        pathToMainFileWhereWillBeSingleSubproject = R"(project_files/testaddtosingle/mainproject_addsingle.libproject)";
        pathToMainFileWhereWillBeNestedSubproject = R"(project_files/testaddtosingle/mainproject_addnested.libproject)";
        pathToMainFileWhichIsTargetForAddBrokenSubproject = R"(project_files/testaddtosingle/mainproject_addbroken.libproject)";
        contentReferenceWithSingle = {
          { "project", "there must be subprojects" },

          { "files", { "main.cpp", "Test.h" } },

          { "subprojects", {"regular/normalsingle.libproject"} }
        };
        contentReferenceWithNested = {
          { "project", "there must be subprojects" },

          { "files", { "main.cpp", "Test.h" } },

          { "subprojects", {"regular/normalnested.libproject"} }
        };
        contentReferenceForTryToAddBroken = {
          { "project", "here must not be any subproject field" },

          { "files", { "main.cpp", "Test.h" } }
        };
        pathToOneRegularSingleSubproject = {
            "project_files/testaddtosingle/regular/normalsingle.libproject"
        };
        pathToOneRegularNestedSubproject = {
            "project_files/testaddtosingle/regular/normalnested.libproject"
        };
        pathToBrokenSubproject = {
            "project_files/testaddtosingle/broken/broken.libproject"
        };

        TestSkeleton::SetUp(); }
    void TearDown() {}
};

class TestAddRegularSubprojectsToNested : public TestSkeleton {
protected:
    string pathToMainFileWhereWillBeSingleSubproject;
    string pathToMainFileWhereWillBeNestedSubproject;
    string pathToMainFileForTestWithVectorFullOfPresentSubprojects;
    string pathToMainFileWhichIsTargetForAddBrokenSubproject;
    string pathToPresentSubproject;
    string pathToMainFileWhereMustBeTwoNewRegularSubprojects;
    string pathToMainFileForTestWithRegularAndBrokenSubprojects;

    json contentReferenceWithSingle;
    json contentReferenceWithNested;
    json contentReferenceForTryToAddBroken;
    json contentReferenceForTestToAddPairOfRegularSubprojects;
    json contentReferenceForTestToAddPairOfRegularAndBrokenSubprojects;
    vector<string> pathToOneRegularSingleSubproject;
    vector<string> pathToOneRegularNestedSubproject;
    vector<string> pathToBrokenSubproject;
    vector<string> pathsToPairOfRegularSubprojects;
    vector<string> pathsToPairOfRegularAndBrokenSubprojects;
    void SetUp() {
        pathToMainFileWhereWillBeSingleSubproject = R"(project_files/testaddtonested/mainproject_addsingle.libproject)";
        pathToMainFileWhereWillBeNestedSubproject = R"(project_files/testaddtonested/mainproject_addnested.libproject)";
        pathToMainFileWhichIsTargetForAddBrokenSubproject = R"(project_files/testaddtonested/mainproject_addbroken.libproject)";
        pathToMainFileForTestWithVectorFullOfPresentSubprojects =
                R"(project_files/testaddtonested/mainproject_addnested_double_add.libproject)";
        pathToPresentSubproject = R"(project_files/testaddtonested/presentsubproject/sub.libproject)";
        pathToMainFileWhereMustBeTwoNewRegularSubprojects =
                R"(project_files/testaddtonested/mainproject_addtworegularsubprojects.libproject)";
        pathToMainFileForTestWithRegularAndBrokenSubprojects =
                R"(project_files/testaddtonested/mainproject_addpairofregularandbroken.libproject)";
        contentReferenceWithSingle = {
          { "project", "there must be subprojects" },

          { "files", { "main.cpp", "Test.h" } },

          { "subprojects", {"presentsubproject/sub.libproject",
                            "regular/normalsingle.libproject"} }
        };
        contentReferenceWithNested = {
          { "project", "there must be subprojects" },

          { "files", { "main.cpp", "Test.h" } },

          { "subprojects", {"presentsubproject/sub.libproject",
                            "regular/normalnested.libproject"} }
        };
        contentReferenceForTryToAddBroken = {
          { "project", "here must not be any subproject field" },

          { "files", { "main.cpp", "Test.h" } }
        };
        contentReferenceForTestToAddPairOfRegularSubprojects = {
            { "project", "there must be subprojects" },

            { "files", { "main.cpp", "Test.h" } },

            { "subprojects", {"presentsubproject/sub.libproject",
                              "fortestwithtworegularsubprojects/single.libproject",
                              "fortestwithtworegularsubprojects/nested.libproject"} }
        };
        contentReferenceForTestToAddPairOfRegularAndBrokenSubprojects = {
            { "project", "there must be subprojects" },

            { "files", { "main.cpp", "Test.h" } },

            { "subprojects", {"presentsubproject/sub.libproject",
                              "fortestwithonebrokenandoneregularsubprojects/normal.libproject"} }
        };
        pathToOneRegularSingleSubproject = {
            "project_files/testaddtonested/regular/normalsingle.libproject"
        };
        pathToOneRegularNestedSubproject = {
            "project_files/testaddtonested/regular/normalnested.libproject"
        };
        pathToBrokenSubproject = {
            "project_files/testaddtonested/broken/broken.libproject"
        };
        pathsToPairOfRegularSubprojects = {
            "project_files/testaddtonested/fortestwithtworegularsubprojects/single.libproject",
            "project_files/testaddtonested/fortestwithtworegularsubprojects/nested.libproject"
        };
        pathsToPairOfRegularAndBrokenSubprojects = {
            "project_files/testaddtonested/fortestwithonebrokenandoneregularsubprojects/normal.libproject",
            "project_files/testaddtonested/fortestwithonebrokenandoneregularsubprojects/broken.libproject"
        };

        TestSkeleton::SetUp(); }
    void TearDown() {}
};

// add one + empty
// add two broken subprojects
// add no subprojects

TEST_F(TestRegularSingle, Open_file) {
  loader = FileSetFactory::createFileSet(PathToFile);
  ASSERT_NO_THROW(loader->open());
}

TEST_F(TestRegularNested, Open_file) {
  loader = FileSetFactory::createFileSet(PathToFile);
  ASSERT_NO_THROW(loader->open());
}

TEST_F(TestRegularSingle, Attemt_to_save_file_without_changes) {
  loader = FileSetFactory::createFileSet(PathToFile);
  loader->open();
  ifstream i;

  json fileToTestBeforeOperation = { };
  i.open(PathToFile);
  if (i.fail())
      throw std::exception();
  fileToTestBeforeOperation << i;
  i.close();

  loader->save();

  json fileToTestAfterOperation = { };
  i.open(PathToFile);
  if (i.fail())
      throw std::exception();
  fileToTestAfterOperation << i;
  i.close();

  ASSERT_EQ(fileToTestBeforeOperation.dump(4), fileToTestAfterOperation.dump(4));
}

TEST_F(TestRegularSingle, Open_file_while_already_loaded) {
  loader = FileSetFactory::createFileSet(PathToFile);
  loader->open();
  ASSERT_THROW(loader->open(), FileSetRuntimeError);
}

TEST_F(TestRegularNested, Get_ACCESS_to_subprojects_if_not_loaded) {
    loader = FileSetFactory::createFileSet(PathToFile);
    map<string, FileSetLoader *> msl;
    ASSERT_THROW(msl = loader->getSubprojectLoaders(), FileSetRuntimeError);
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
  string path = loader->getPathToNode();
  ASSERT_EQ(PathToFile, path);
}

TEST_F(TestRegularNested, Get_path_to_1st_subnode) {
    loader = FileSetFactory::createFileSet(PathToFile);
    loader->open();
    map<string, FileSetLoader *> msl = loader->getSubprojectLoaders();
    map<string, FileSetLoader *>::const_iterator it = msl.cbegin();
    string path = (*it).second->getPathToNode();
    ASSERT_EQ(dirPath + subprojectsFilesRef.at(0), path);
}

TEST_F(TestRegularNested, Get_path_to_2n_subnode) {
    loader = FileSetFactory::createFileSet(PathToFile);
    loader->open();
    map<string, FileSetLoader *> msl = loader->getSubprojectLoaders();
    map<string, FileSetLoader *>::const_reverse_iterator it = msl.crbegin();
    //crbegin because we have only 2 item in list
    string path = (*it).second->getPathToNode();
    ASSERT_EQ(dirPath + subprojectsFilesRef.at(1), path);
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

TEST_F(TestRegularNested, Get_project_name_for_1st_subproject) {
    loader = FileSetFactory::createFileSet(PathToFile);
    loader->open();
    map<string, FileSetLoader *> msl = loader->getSubprojectLoaders();
    map<string, FileSetLoader *>::const_iterator it = msl.cbegin();
    string name = (*it).second->getProjectName();
    ASSERT_EQ(subprojectsNamesRef.at(0), name);
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

TEST_F(TestRegularSingle, Count_subprojects_of_single_project) {
    loader = FileSetFactory::createFileSet(PathToFile);
    loader->open();
    ASSERT_EQ(0, loader->countSubprojects());
}

TEST_F(TestRegularNested, Count_subprojects_of_nested_project) {
    loader = FileSetFactory::createFileSet(PathToFile);
    loader->open();
    ASSERT_EQ(subprojectsCount, loader->countSubprojects());
}

TEST_F(TestRegularSingle, Count_subprojects_of_sinlge_not_loaded) {
    loader = FileSetFactory::createFileSet(PathToFile);
    ASSERT_THROW(loader->countSubprojects(), FileSetRuntimeError);
}

TEST_F(TestAddRegularSubprojectsToSingle, Add_one_regular_single_subproject) {
    json fileToTest = { };
    ASSERT_NO_THROW({
                        loader = FileSetFactory::createFileSet(pathToMainFileWhereWillBeSingleSubproject);
                        loader->open();
                        loader->addSubprojects(pathToOneRegularSingleSubproject);
                        loader->save();
                        ifstream i(pathToMainFileWhereWillBeSingleSubproject);
                        if (i.fail())
                            throw std::exception();
                        fileToTest << i;
                 });
    ASSERT_EQ(contentReferenceWithSingle.dump(4), fileToTest.dump(4));
}

TEST_F(TestAddRegularSubprojectsToSingle, Add_one_regular_nested_subproject) {
    json fileToTest = { };
    ASSERT_NO_THROW({
                        loader = FileSetFactory::createFileSet(pathToMainFileWhereWillBeNestedSubproject);
                        loader->open();
                        loader->addSubprojects(pathToOneRegularNestedSubproject);
                        loader->save();
                        ifstream i(pathToMainFileWhereWillBeNestedSubproject);
                        if (i.fail())
                            throw std::exception();
                        fileToTest << i;
                 });
    ASSERT_EQ(contentReferenceWithNested.dump(4), fileToTest.dump(4));
}

TEST_F(TestAddRegularSubprojectsToSingle, Add_broken_subproject) {
    json fileToTest = { };
    ASSERT_NO_THROW({
                        loader = FileSetFactory::createFileSet(pathToMainFileWhichIsTargetForAddBrokenSubproject);
                        loader->open();
                        loader->addSubprojects(pathToBrokenSubproject);
                        ifstream i(pathToMainFileWhichIsTargetForAddBrokenSubproject);
                        if (i.fail())
                            throw std::exception();
                        fileToTest << i;
                    });

    ASSERT_EQ(contentReferenceForTryToAddBroken.dump(4), fileToTest.dump(4));
}

TEST_F(TestAddRegularSubprojectsToSingle, Add_one_regular_single_which_already_cached) {
    json fileToTest = { };
    ASSERT_NO_THROW({
                        loader = FileSetFactory::createFileSet(pathToMainFileWhereWillBeSingleSubproject);
                        loader->open();
                        loader->addSubprojects(pathToOneRegularSingleSubproject);
                        loader->addSubprojects(pathToOneRegularSingleSubproject);
                        loader->save();
                        ifstream i(pathToMainFileWhereWillBeSingleSubproject);
                        if (i.fail())
                            throw std::exception();
                        fileToTest << i;
                 });
    ASSERT_EQ(contentReferenceWithSingle.dump(4), fileToTest.dump(4));
}

TEST_F(TestAddRegularSubprojectsToNested, Add_one_regular_single_subproject) {
    json fileToTest = { };
    ASSERT_NO_THROW({
                        loader = FileSetFactory::createFileSet(pathToMainFileWhereWillBeSingleSubproject);
                        loader->open();
                        loader->addSubprojects(pathToOneRegularSingleSubproject);
                        loader->save();
                        ifstream i(pathToMainFileWhereWillBeSingleSubproject);
                        if (i.fail())
                            throw std::exception();
                        fileToTest << i;
                 });
    ASSERT_EQ(contentReferenceWithSingle.dump(4), fileToTest.dump(4));
}

TEST_F(TestAddRegularSubprojectsToNested, Add_one_regular_nested_subproject) {
    json fileToTest = { };
    ASSERT_NO_THROW({
                        loader = FileSetFactory::createFileSet(pathToMainFileWhereWillBeNestedSubproject);
                        loader->open();
                        loader->addSubprojects(pathToOneRegularNestedSubproject);
                        loader->save();
                        ifstream i(pathToMainFileWhereWillBeNestedSubproject);
                        if (i.fail())
                            throw std::exception();
                        fileToTest << i;
                 });
    ASSERT_EQ(contentReferenceWithNested.dump(4), fileToTest.dump(4));
}

TEST_F(TestAddRegularSubprojectsToNested, Add_broken_subproject) {
    json fileToTest = { };
    ASSERT_NO_THROW({
                        loader = FileSetFactory::createFileSet(pathToMainFileWhichIsTargetForAddBrokenSubproject);
                        loader->open();
                        loader->addSubprojects(pathToBrokenSubproject);
                        ifstream i(pathToMainFileWhichIsTargetForAddBrokenSubproject);
                        if (i.fail())
                            throw std::exception();
                        fileToTest << i;
                    });

    ASSERT_EQ(contentReferenceForTryToAddBroken.dump(4), fileToTest.dump(4));
}

TEST_F(TestAddRegularSubprojectsToNested, Add_one_regular_single_which_already_cached) {
    json fileToTest = { };
    ASSERT_NO_THROW({
                        loader = FileSetFactory::createFileSet(pathToMainFileWhereWillBeSingleSubproject);
                        loader->open();
                        loader->addSubprojects(pathToOneRegularSingleSubproject);
                        loader->addSubprojects(pathToOneRegularSingleSubproject);
                        loader->save();
                        ifstream i(pathToMainFileWhereWillBeSingleSubproject);
                        if (i.fail())
                            throw std::exception();
                        fileToTest << i;
                 });
    ASSERT_EQ(contentReferenceWithSingle.dump(4), fileToTest.dump(4));
}

TEST_F(TestAddRegularSubprojectsToNested, Add_one_regular_single_which_already_present_in_project_file) {
    json fileToTest = { };
    ASSERT_NO_THROW({
                        loader = FileSetFactory::createFileSet(pathToMainFileWhereWillBeSingleSubproject);
                        loader->open();
                        loader->addSubprojects(pathToOneRegularSingleSubproject);
                        loader->save();
                        loader->addSubprojects(pathToOneRegularSingleSubproject);
                        loader->save();
                        ifstream i(pathToMainFileWhereWillBeSingleSubproject);
                        if (i.fail())
                            throw std::exception();
                        fileToTest << i;
                 });
    ASSERT_EQ(contentReferenceWithSingle.dump(4), fileToTest.dump(4));
}

TEST_F(TestAddRegularSubprojectsToNested, Add_already_cached_and_present_subprojects) {
    json fileToTest = { };
    vector<string> subprojectsToAdd = pathToOneRegularNestedSubproject;
    subprojectsToAdd.push_back(pathToPresentSubproject);

    ASSERT_NO_THROW({
                        loader = FileSetFactory::createFileSet(pathToMainFileForTestWithVectorFullOfPresentSubprojects);
                        loader->open();
                        loader->addSubprojects(pathToOneRegularNestedSubproject);
                        loader->addSubprojects(subprojectsToAdd);
                        loader->save();
                        ifstream i(pathToMainFileForTestWithVectorFullOfPresentSubprojects);
                        if (i.fail())
                            throw std::exception();
                        fileToTest << i;
                 });
    ASSERT_EQ(contentReferenceWithNested.dump(4), fileToTest.dump(4));
}

TEST_F(TestAddRegularSubprojectsToNested, Add_two_regular_subprojects) {
    json fileToTest = { };
    ASSERT_NO_THROW({
                        loader = FileSetFactory::createFileSet(pathToMainFileWhereMustBeTwoNewRegularSubprojects);
                        loader->open();
                        loader->addSubprojects(pathsToPairOfRegularSubprojects);
                        loader->save();
                        ifstream i(pathToMainFileWhereMustBeTwoNewRegularSubprojects);
                        if (i.fail())
                            throw std::exception();
                        fileToTest << i;
                 });
    ASSERT_EQ(contentReferenceForTestToAddPairOfRegularSubprojects.dump(4), fileToTest.dump(4));
}

TEST_F(TestAddRegularSubprojectsToNested, Add_regular_and_broken_subprojects) {
    json fileToTest = { };
    ASSERT_NO_THROW({
                        loader = FileSetFactory::createFileSet(pathToMainFileForTestWithRegularAndBrokenSubprojects);
                        loader->open();
                        loader->addSubprojects(pathsToPairOfRegularAndBrokenSubprojects);
                        loader->save();
                        ifstream i(pathToMainFileForTestWithRegularAndBrokenSubprojects);
                        if (i.fail())
                            throw std::exception();
                        fileToTest << i;
                 });
    ASSERT_EQ(contentReferenceForTestToAddPairOfRegularAndBrokenSubprojects.dump(4), fileToTest.dump(4));
}

} // namespace FileSetTests

int main(int argc, char **argv) {
  using namespace FileSetTests;
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
