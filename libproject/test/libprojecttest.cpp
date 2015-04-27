#include <string>
#include <list>
#include <gtest/gtest.h>
#include "libproject.h"
#include "libproject_error.h"
#include <map>
#include <vector>
#include "json.hpp"
#include <fstream>
#include <sstream>

using std::string;
using std::list;
using LibprojManager::Interface::FileSetFactory;
using LibprojManager::Interface::FileSetLoader;
using namespace LibprojManager::Interface::Error;
using std::map;
using std::vector;
using nlohmann::json;
using std::ifstream;
using std::ostringstream;
using std::ofstream;

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
    json fileToTest = { };
    string pathToMainFile;
    string contentBackup;

    string pathToMainFileWhichIsTargetForAddBrokenSubproject;
    json contentReferenceWithSingle;
    json contentReferenceWithNested;
    json contentReference;
    json contentReferenceForTryToAddBroken;

    string pathToOneRegularSingleSubproject;
    string pathToOneRegularNestedSubproject;
    vector<string> pathToBrokenSubproject;
    void SetUp() {
        pathToMainFile = R"(project_files/testaddtosingle/mainproject_addsingle.libproject)";
        pathToMainFileWhichIsTargetForAddBrokenSubproject = R"(project_files/testaddtosingle/mainproject_addbroken.libproject)";
        contentReference = {
          { "project", "there must be subprojects" },

          { "files", { "main.cpp", "Test.h" } }
        };
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

        pathToOneRegularSingleSubproject =
            R"(project_files/testaddtosingle/regular/normalsingle.libproject)";

        pathToOneRegularNestedSubproject =
            R"(project_files/testaddtosingle/regular/normalnested.libproject)";

        pathToBrokenSubproject = {
            "project_files/testaddtosingle/broken/broken.libproject"
        };

        ifstream in(pathToMainFile);
        ostringstream os;
        char buf;
        while(in.get(buf))
          os << buf;
        in.close();
        contentBackup = os.str();

        TestSkeleton::SetUp(); }

    void TearDown() {
      ofstream out (pathToMainFile);
      for (const auto& sym : contentBackup)
        out << sym;
      out.close();
    }
};

class TestAddRegularSubprojectsToNested : public TestSkeleton {
protected:
    json fileToTest = { };
    string pathToMainFile;
    string contentBackup;

    string pathToPresentSubproject;

    json contentReferenceWithNewSingle;
    json contentReferenceWithNewNested;
    json contentReference;
    json contentReferenceWithPairOfNewRegularSubprojects;


    string pathToOneRegularSingleSubproject;
    string pathToOneRegularNestedSubproject;
    vector<string> pathToBrokenSubproject;
    vector<string> pathsToPairOfRegularSubprojects;
    vector<string> pathsToPairOfRegularAndBrokenSubprojects;
    vector<string> pathsToPairOfBrokenSubprojects;
    vector<string> pathsToPairOfRegularAndEmptySubprojects;

    void SetUp() {
        pathToMainFile = R"(project_files/testaddtonested/mainproject_addnested.libproject)";
        pathToPresentSubproject = R"(project_files/testaddtonested/presentsubproject/sub.libproject)";


        contentReferenceWithNewSingle = {
          { "project", "there must be subprojects" },

          { "files", { "main.cpp", "Test.h" } },

          { "subprojects", {"presentsubproject/sub.libproject",
                            "regular/normalsingle.libproject"} }
        };
        contentReferenceWithNewNested = {
          { "project", "there must be subprojects" },

          { "files", { "main.cpp", "Test.h" } },

          { "subprojects", {"presentsubproject/sub.libproject",
                            "regular/normalnested.libproject"} }
        };
        contentReference = {
          { "project", "there must be subprojects" },

          { "files", { "main.cpp", "Test.h" } },

          { "subprojects", {"presentsubproject/sub.libproject"} }
        };
        contentReferenceWithPairOfNewRegularSubprojects = {
            { "project", "there must be subprojects" },

            { "files", { "main.cpp", "Test.h" } },

            { "subprojects", {"presentsubproject/sub.libproject",
                              "fortestwithtworegularsubprojects/nested.libproject",
                              "fortestwithtworegularsubprojects/single.libproject"} }
        };
        pathToOneRegularSingleSubproject =
            R"(project_files/testaddtonested/regular/normalsingle.libproject)";

        pathToOneRegularNestedSubproject =
            R"(project_files/testaddtonested/regular/normalnested.libproject)";

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
        pathsToPairOfBrokenSubprojects = {
            "project_files/testaddtonested/fortestwithtwobrokensubprojects/one.libproject",
            "project_files/testaddtonested/fortestwithtwobrokensubprojects/two.libproject"
        };
        pathsToPairOfRegularAndEmptySubprojects = {
            "project_files/testaddtonested/fortestwithoneregularandemptysubprojects/regular.libproject",
            "project_files/testaddtonested/fortestwithoneregularandemptysubprojects/empty.libproject"
        };

        ifstream in(pathToMainFile);
        ostringstream os;
        char buf;
        while(in.get(buf))
          os << buf;
        in.close();
        contentBackup = os.str();

        TestSkeleton::SetUp();
    }

    void TearDown() {
      ofstream out (pathToMainFile);
      for (const auto& sym : contentBackup)
        out << sym;
      out.close();
    }
};

class TestRemoveSubprojects : public TestSkeleton {
protected:
    json fileToTest = { };
    string pathToMainFileWhereNeedToRemoveOneSubprojectInArrayOfTwo;
    string pathToMainFileWhereNeedToRemoveOneNonExistentSubproject;
    string pathToMainFileWhereNeedToRemoveTwoNonExistentSubprojects;
    string pathToMainFileWhereNeedToRemoveTwoSubprojectsWithEqualButCorrectPaths;
    string pathToMainFileWhereIsOneSubprojectWithWillBeRemoved;
    string pathToMainFileWhereNeedToRemoveOneExistentAndOneNonExistentSubprojects;
    string pathToMainFileWhichWillNotBeLoaded;
    string pathToMainFileWhereWillBeRemoveAndCountActions;

    string contentBackup_oneSubproject;
    string contentBackup_withoutSubprojects;

    json contentReference_withOneSuproject;
    json contentReference_withTwoSuprojects;
    json contentReference_withoutSuprojects;

    string pathToPresentSubproject;
    string pathToNonPresentSubproject;
    vector<string> pathsToNonPresentSubprojects;
    vector<string> pathsDuplicated;
    vector<string> pathsToPresentAndNonPresentSubprojects;

    void SetUp() {
        pathToMainFileWhereNeedToRemoveOneSubprojectInArrayOfTwo =
                R"(project_files/testremove/case_0.libproject)";
        pathToMainFileWhereNeedToRemoveOneNonExistentSubproject =
                R"(project_files/testremove/case_1.libproject)";
        pathToMainFileWhereNeedToRemoveTwoNonExistentSubprojects =
                R"(project_files/testremove/case_2.libproject)";
        pathToMainFileWhereNeedToRemoveTwoSubprojectsWithEqualButCorrectPaths =
                R"(project_files/testremove/case_3.libproject)";
        pathToMainFileWhereIsOneSubprojectWithWillBeRemoved =
                R"(project_files/testremove/case_4.libproject)";
        pathToMainFileWhereNeedToRemoveOneExistentAndOneNonExistentSubprojects =
                R"(project_files/testremove/case_5.libproject)";
        pathToMainFileWhichWillNotBeLoaded =
                R"(project_files/testremove/case_6.libproject)";
        pathToMainFileWhereWillBeRemoveAndCountActions =
                R"(project_files/testremove/case_7.libproject)";

        contentReference_withOneSuproject = {
            { "project", "try to remove" },

            { "files", { "main.cpp", "Test.h", "Test.cpp" } },

            { "subprojects", {"sub/s2.libproject"} }
          };
        contentReference_withTwoSuprojects = {
            { "project", "try to remove" },

            { "files", { "main.cpp", "Test.h", "Test.cpp" } },

            { "subprojects", {"sub/s1.libproject", "sub/s2.libproject"} }
          };
        contentReference_withoutSuprojects = {
            { "project", "try to remove" },

            { "files", { "main.cpp", "Test.h", "Test.cpp" } }
          };

        pathToPresentSubproject = "sub/s1.libproject";
        pathToNonPresentSubproject = "subs/s1.libproject";
        pathsToNonPresentSubprojects = vector<string>({ pathToNonPresentSubproject, "sub/nonexistent.libproject" });
        pathsDuplicated = vector<string>({ pathToPresentSubproject, pathToPresentSubproject });
        pathsToPresentAndNonPresentSubprojects = vector<string>({ pathToPresentSubproject, "sub/nonexistent.libproject" });

        //backup content of case 0
        ifstream in(pathToMainFileWhereNeedToRemoveOneSubprojectInArrayOfTwo);
        ostringstream os_case_removing_subroject_from_array_of_two;
        char buf;
        while(in.get(buf))
          os_case_removing_subroject_from_array_of_two << buf;
        in.close();
        contentBackup_oneSubproject = os_case_removing_subroject_from_array_of_two.str();

        //backup content of case 4
        in.open(pathToMainFileWhereIsOneSubprojectWithWillBeRemoved);
        ostringstream os_case_removing_lone_subproject;
        buf = 0;
        while(in.get(buf))
          os_case_removing_lone_subproject << buf;
        in.close();
        contentBackup_withoutSubprojects = os_case_removing_lone_subproject.str();

        TestSkeleton::SetUp();
    }
    void TearDown() {

      //reverting backups:
      ofstream out_case_removing_subroject_from_array_of_two (pathToMainFileWhereNeedToRemoveOneSubprojectInArrayOfTwo);
      for (const auto& sym : contentBackup_oneSubproject)
        out_case_removing_subroject_from_array_of_two << sym;
      out_case_removing_subroject_from_array_of_two.close();

      ofstream out_case_removing_lone_subproject (pathToMainFileWhereIsOneSubprojectWithWillBeRemoved);
      for (const auto& sym : contentBackup_withoutSubprojects)
        out_case_removing_lone_subproject << sym;
      out_case_removing_lone_subproject.close();
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

TEST_F(TestRegularSingle, Attemt_to_save_file_without_changes) {
  loader = FileSetFactory::createFileSet(PathToFile);
  loader->open();
  ifstream i;

  json fileToTestBeforeOperation = { };
  i.open(PathToFile);
  fileToTestBeforeOperation << i;
  i.close();

  loader->save();

  json fileToTestAfterOperation = { };
  i.open(PathToFile);
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
               std::exception);
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
    ASSERT_NO_THROW({
                        loader = FileSetFactory::createFileSet(pathToMainFile);
                        loader->open();
                        loader->addSubproject(pathToOneRegularSingleSubproject);
                        loader->save();
                        ifstream i(pathToMainFile);
                        fileToTest << i;
                 });
    ASSERT_EQ(contentReferenceWithSingle.dump(4), fileToTest.dump(4));
}

TEST_F(TestAddRegularSubprojectsToSingle, Add_one_regular_nested_subproject) {
    ASSERT_NO_THROW({
                        loader = FileSetFactory::createFileSet(pathToMainFile);
                        loader->open();
                        loader->addSubproject(pathToOneRegularNestedSubproject);
                        loader->save();
                        ifstream i(pathToMainFile);
                        fileToTest << i;
                 });
    ASSERT_EQ(contentReferenceWithNested.dump(4), fileToTest.dump(4));
}

TEST_F(TestAddRegularSubprojectsToSingle, Add_broken_subproject) {
    ASSERT_THROW({
                        loader = FileSetFactory::createFileSet(pathToMainFile);
                        loader->open();
                        loader->addSubprojects(pathToBrokenSubproject);
                 }, std::exception);
    loader->save();
    ifstream i(pathToMainFile);
    fileToTest << i;

    ASSERT_EQ(contentReference.dump(4), fileToTest.dump(4));
}

TEST_F(TestAddRegularSubprojectsToSingle, Add_one_regular_single_which_already_cached) {
    ASSERT_THROW({
                        loader = FileSetFactory::createFileSet(pathToMainFile);
                        loader->open();
                        loader->addSubproject(pathToOneRegularSingleSubproject);
                        loader->addSubproject(pathToOneRegularSingleSubproject);
                 }, FileSetRuntimeError);
    loader->save();
    ifstream i(pathToMainFile);
    fileToTest << i;

    ASSERT_EQ(contentReferenceWithSingle.dump(4), fileToTest.dump(4));
}

TEST_F(TestAddRegularSubprojectsToNested, Add_one_regular_single_subproject) {
    ASSERT_NO_THROW({
                        loader = FileSetFactory::createFileSet(pathToMainFile);
                        loader->open();
                        loader->addSubproject(pathToOneRegularSingleSubproject);
                        loader->save();
                        ifstream i(pathToMainFile);
                        fileToTest << i;
                 });

    ASSERT_EQ(contentReferenceWithNewSingle.dump(4), fileToTest.dump(4));
}

TEST_F(TestAddRegularSubprojectsToNested, Add_one_regular_nested_subproject) {
    ASSERT_NO_THROW({
                        loader = FileSetFactory::createFileSet(pathToMainFile);
                        loader->open();
                        loader->addSubproject(pathToOneRegularNestedSubproject);
                        loader->save();
                        ifstream i(pathToMainFile);
                        fileToTest << i;
                 });
    ASSERT_EQ(contentReferenceWithNewNested.dump(4), fileToTest.dump(4));
}

TEST_F(TestAddRegularSubprojectsToNested, Add_broken_subproject) {
    ASSERT_THROW({
                        loader = FileSetFactory::createFileSet(pathToMainFile);
                        loader->open();
                        loader->addSubprojects(pathToBrokenSubproject);
                 }, std::exception);
    loader->save();
    ifstream i(pathToMainFile);
    fileToTest << i;

    ASSERT_EQ(contentReference.dump(4), fileToTest.dump(4));
}

TEST_F(TestAddRegularSubprojectsToNested, Add_one_regular_single_which_already_cached) {
    ASSERT_THROW({
                        loader = FileSetFactory::createFileSet(pathToMainFile);
                        loader->open();
                        loader->addSubproject(pathToOneRegularSingleSubproject);
                        loader->addSubproject(pathToOneRegularSingleSubproject);
                 }, FileSetRuntimeError);
    ifstream i(pathToMainFile);
    fileToTest << i;

    ASSERT_EQ(contentReference.dump(4), fileToTest.dump(4));
}

TEST_F(TestAddRegularSubprojectsToNested, Add_one_regular_single_which_already_present_in_project_file) {
    ASSERT_THROW({
                        loader = FileSetFactory::createFileSet(pathToMainFile);
                        loader->open();
                        loader->addSubproject(pathToOneRegularSingleSubproject);
                        loader->save();
                        loader->addSubproject(pathToOneRegularSingleSubproject);
                    }, FileSetRuntimeError);
    loader->save();
    ifstream i(pathToMainFile);
    fileToTest << i;

    ASSERT_EQ(contentReferenceWithNewSingle.dump(4), fileToTest.dump(4));
}

TEST_F(TestAddRegularSubprojectsToNested, Add_already_cached_and_present_subprojects) {
    vector<string> subprojectsToAdd = {pathToOneRegularNestedSubproject, pathToPresentSubproject};

    ASSERT_NO_THROW({
                        loader = FileSetFactory::createFileSet(pathToMainFile);
                        loader->open();
                        loader->addSubproject(pathToOneRegularNestedSubproject);
                        loader->save();
                      });
    ASSERT_THROW({
                        loader->addSubprojects(subprojectsToAdd);
                 }, FileSetRuntimeError);
    loader->save();
    ifstream i(pathToMainFile);
    fileToTest << i;

    ASSERT_EQ(contentReferenceWithNewNested.dump(4), fileToTest.dump(4));
}

TEST_F(TestAddRegularSubprojectsToNested, Add_two_regular_subprojects) {
    ASSERT_NO_THROW({
                        loader = FileSetFactory::createFileSet(pathToMainFile);
                        loader->open();
                        loader->addSubprojects(pathsToPairOfRegularSubprojects);
                        loader->save();
                        ifstream i(pathToMainFile);
                        fileToTest << i;
                 });
    ASSERT_EQ(contentReferenceWithPairOfNewRegularSubprojects.dump(4), fileToTest.dump(4));
}

TEST_F(TestAddRegularSubprojectsToNested, Add_regular_and_broken_subprojects) {
    ASSERT_THROW({
                        loader = FileSetFactory::createFileSet(pathToMainFile);
                        loader->open();
                        loader->addSubprojects(pathsToPairOfRegularAndBrokenSubprojects);
                 }, std::exception);
    loader->save();
    ifstream i(pathToMainFile);
    fileToTest << i;

    ASSERT_EQ(contentReference.dump(4), fileToTest.dump(4));
}

TEST_F(TestAddRegularSubprojectsToNested, Add_pair_of_broken_subprojects) {
    ASSERT_THROW({
                        loader = FileSetFactory::createFileSet(pathToMainFile);
                        loader->open();
                        loader->addSubprojects(pathsToPairOfBrokenSubprojects);
                 }, std::exception);
    loader->save();
    ifstream i(pathToMainFile);
    fileToTest << i;

    ASSERT_EQ(contentReference.dump(4), fileToTest.dump(4));
}

TEST_F(TestAddRegularSubprojectsToNested, Add_regular_and_empty_subprojects) {
    ASSERT_THROW({
                        loader = FileSetFactory::createFileSet(pathToMainFile);
                        loader->open();
                        loader->addSubprojects(pathsToPairOfRegularAndEmptySubprojects);
                    }, std::exception);
    loader->save();
    ifstream i(pathToMainFile);
    fileToTest << i;

    ASSERT_EQ(contentReference.dump(4), fileToTest.dump(4));
}

TEST_F(TestAddRegularSubprojectsToNested, Add_empty_vector_of_subprojects) {
    ASSERT_NO_THROW({
                        loader = FileSetFactory::createFileSet(pathToMainFile);
                        loader->open();
                        loader->addSubprojects(vector<string>());
                        loader->save();
                        ifstream i(pathToMainFile);
                        fileToTest << i;
                    });
    ASSERT_EQ(contentReference.dump(4), fileToTest.dump(4));
}

TEST_F(TestRemoveSubprojects, Remove_one_subproject_from_file_with_two) {
    ASSERT_NO_THROW({
                        loader = FileSetFactory::createFileSet(pathToMainFileWhereNeedToRemoveOneSubprojectInArrayOfTwo);
                        loader->open();
                        loader->removeSubproject(pathToPresentSubproject);
                        loader->save();
                        ifstream i(pathToMainFileWhereNeedToRemoveOneSubprojectInArrayOfTwo);
                        fileToTest << i;
                    });
    ASSERT_EQ(contentReference_withOneSuproject.dump(4), fileToTest.dump(4));
}

TEST_F(TestRemoveSubprojects, Remove_lone_subproject) {
    ASSERT_NO_THROW({
                        loader = FileSetFactory::createFileSet(pathToMainFileWhereIsOneSubprojectWithWillBeRemoved);
                        loader->open();
                        loader->removeSubproject(pathToPresentSubproject);
                        loader->save();
                        ifstream i(pathToMainFileWhereIsOneSubprojectWithWillBeRemoved);
                        fileToTest << i;
                    });
    ASSERT_EQ(contentReference_withoutSuprojects.dump(4), fileToTest.dump(4));
}

TEST_F(TestRemoveSubprojects, Remove_nonexistent_subproject) {
    ASSERT_THROW({
                        loader = FileSetFactory::createFileSet(pathToMainFileWhereNeedToRemoveOneNonExistentSubproject);
                        loader->open();
                        loader->removeSubproject(pathToNonPresentSubproject);
                 }, FileSetRuntimeError);
    loader->save();
    ifstream i(pathToMainFileWhereNeedToRemoveOneNonExistentSubproject);
    fileToTest << i;

    ASSERT_EQ(contentReference_withTwoSuprojects.dump(4), fileToTest.dump(4));
}

TEST_F(TestRemoveSubprojects, Remove_nonexistent_subprojects) {
    ASSERT_THROW({
                        loader = FileSetFactory::createFileSet(pathToMainFileWhereNeedToRemoveTwoNonExistentSubprojects);
                        loader->open();
                        loader->removeSubprojects(pathsToNonPresentSubprojects);
                 }, FileSetRuntimeError);
    loader->save();
    ifstream i(pathToMainFileWhereNeedToRemoveTwoNonExistentSubprojects);
    fileToTest << i;

    ASSERT_EQ(contentReference_withTwoSuprojects.dump(4), fileToTest.dump(4));
}

TEST_F(TestRemoveSubprojects, Remove_two_equal_correct_subprojects) {
    ASSERT_THROW({
                        loader = FileSetFactory::createFileSet(pathToMainFileWhereNeedToRemoveTwoSubprojectsWithEqualButCorrectPaths);
                        loader->open();
                        loader->removeSubprojects(pathsDuplicated);
                 }, FileSetRuntimeError);
    loader->save();
    ifstream i(pathToMainFileWhereNeedToRemoveTwoSubprojectsWithEqualButCorrectPaths);
    fileToTest << i;

    ASSERT_EQ(contentReference_withTwoSuprojects.dump(4), fileToTest.dump(4));
}

TEST_F(TestRemoveSubprojects, Remove_existent_and_nonexistent_subprojects) {
    ASSERT_THROW({
                        loader = FileSetFactory::createFileSet(pathToMainFileWhereNeedToRemoveOneExistentAndOneNonExistentSubprojects);
                        loader->open();
                        loader->removeSubprojects(pathsToPresentAndNonPresentSubprojects);
                 }, FileSetRuntimeError);
    loader->save();
    ifstream i(pathToMainFileWhereNeedToRemoveOneExistentAndOneNonExistentSubprojects);
    fileToTest << i;

    ASSERT_EQ(contentReference_withTwoSuprojects.dump(4), fileToTest.dump(4));
}

TEST_F(TestRemoveSubprojects, Remove_on_not_loaded_interface) {
    ASSERT_THROW({
                        loader = FileSetFactory::createFileSet(pathToMainFileWhichWillNotBeLoaded);
                        loader->removeSubproject(pathToPresentSubproject);
                 }, FileSetRuntimeError);
    loader->save();
    ifstream i(pathToMainFileWhichWillNotBeLoaded);
    fileToTest << i;

    ASSERT_EQ(contentReference_withTwoSuprojects.dump(4), fileToTest.dump(4));
}

// TODO Count_subprojects_after_removing

//TODO test with pair broken path and regular

//TODO Add tests for function getSubprojectsPaths() !!!!!!!!!!!!
//TODO Add tests fot findSubprojectByPath() !!!!!!!!!!!!

} // namespace FileSetTests

int main(int argc, char **argv) {
  using namespace FileSetTests;
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
