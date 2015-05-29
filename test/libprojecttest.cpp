#include <string>
#include <list>
#include <gtest/gtest.h>
#include "libproject/libproject.h"
#include "libproject/libproject_error.h"
#include <map>
#include <vector>
#include "json.hpp"
#include <fstream>
#include <sstream>
#include <memory>

using namespace LibprojManager::Interface;
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
using std::unique_ptr;



namespace {
list<FileSetLoader::Path> pathsToSingleAbnormal = {
    "project_files/single/empty.libproject",
    "project_files/single/notrelevantfileskey.libproject",
    "project_files/single/filesarrayisdigit.libproject",
    "project_files/single/filesarrayisstring.libproject",
    "project_files/single/nofileskey.libproject",
    "project_files/single/broken.libproject",
    "project_files/single/noprojectkey.libproject",
    "project_files/single/notexist.libproject"
};

vector<FileSetLoader::Subprojects> pathsToAbnormalSubprojects = {
    {
        "project_files/testaddtonested/broken/broken.libproject"
    },
    {
        "project_files/testaddtonested/fortestwithonebrokenandoneregularsubprojects/normal.libproject",
        "project_files/testaddtonested/fortestwithonebrokenandoneregularsubprojects/broken.libproject"
    },
    {   "project_files/testaddtonested/fortestwithtwobrokensubprojects/one.libproject",
        "project_files/testaddtonested/fortestwithtwobrokensubprojects/two.libproject"
    },
    {
        "project_files/testaddtonested/fortestwithoneregularandemptysubprojects/regular.libproject",
        "project_files/testaddtonested/fortestwithoneregularandemptysubprojects/empty.libproject"
    },
    {
    }
};

string pathToNestedMainFile = R"(project_files/testaddtonested/mainproject_addnested.libproject)";

vector<FileSetLoader::Subprojects> incorrectPathsOfSubprojects = {
    //one non-existent
    {
        "subs/s1.libproject"
    },

    //two non-existent
    {
        "subs/s1.libproject",
        "sub/nonexistent.libproject"
    },

    //pair of existent
    {
        "sub/s1.libproject",
        "sub/s1.libproject"
    },

    //one existent, one non-existent
    {
        "sub/s1.libproject",
        "sub/nonexistent.libproject"
    }

};
}

namespace FileSetTests {

// Skeleton classes:
class TestSkeleton : public ::testing::Test {
public:
  typedef unique_ptr<FileSetLoader> LoaderPtr;

  virtual ~TestSkeleton() {}
  json contentReference;
  json contentReferenceWithSingle;
  json contentReferenceWithNested;
  string contentBackup;
  LoaderPtr loader;
  json fileToTest = { };
  FileSetLoader::Path pathToMainFile;
  bool writableMainFile = false;
  virtual void SetUp() {
    if (writableMainFile)
    {
        ifstream in(pathToMainFile);
        ostringstream os;
        char buf;
        while(in.get(buf))
            os << buf;
        in.close();
        contentBackup = os.str();
    }
  }

  virtual void TearDown() {
      if (writableMainFile)
      {
        ofstream out (pathToMainFile);
        for (const auto& sym : contentBackup)
            out << sym;
        out.close();
      }

  }
};

class TestAddSubprojects : public TestSkeleton {
public:
    void SetUp() {
        writableMainFile = true;
        TestSkeleton::SetUp();
    }
    void TearDown() {
        TestSkeleton::TearDown();
    }
};

class TestRegular : public TestSkeleton {
public:
    string projectNameRef;
    FileSetLoader::Files projectFilesRef;
    void SetUp() {
        TestSkeleton::SetUp();
    }
    void TearDown() {
        TestSkeleton::TearDown();
    }
};

class TestProjectsWithFewSubprojects : public TestSkeleton {
public:
    json contentReference_withOneSuproject = {
        { "project", "try to remove" },
        { "files", { "main.cpp", "Test.h", "Test.cpp" } },
        { "subprojects", {"sub/s2.libproject"} }
  };
    json contentReference_withTwoSuprojects = {
        { "project", "try to remove" },
        { "files", { "main.cpp", "Test.h", "Test.cpp" } },
        { "subprojects", {"sub/s1.libproject", "sub/s2.libproject"} }
  };
    json contentReference_withoutSuprojects = {
        { "project", "try to remove" },
        { "files", { "main.cpp", "Test.h", "Test.cpp" } }
  };

    void SetUp() {
        TestSkeleton::SetUp();
    }
    void TearDown() {
        TestSkeleton::TearDown();
    }
};





// classes-workers:
class TestRegularSingle : public TestRegular {
public:
  void SetUp() {
    pathToMainFile = {"project_files/single/normal.libproject"};
    projectNameRef = {"SameAmmoniteOnPurpleSkies *** 42"};
    projectFilesRef = {"main.cpp",    "Test.h",      "Test.cpp",
                       "Foo.h",       "Foo.cpp",     "README",
                       "kekeke.cpp",  "testnew.cpp", "testadd2.cpp",
                       "testadd3.cpp"};
    TestRegular::SetUp();
  }
  void TearDown() {
      TestRegular::TearDown();
  }
};

TEST_F(TestRegularSingle, Open_file) {
  loader = LoaderPtr(FileSetFactory::createFileSet(pathToMainFile));
  ASSERT_NO_THROW(loader->open());
}

TEST_F(TestRegularSingle, Attemt_to_save_file_without_changes) {
  loader = LoaderPtr(FileSetFactory::createFileSet(pathToMainFile));
  loader->open();
  ifstream i;

  json fileToTestBeforeOperation = { };
  i.open(pathToMainFile);
  fileToTestBeforeOperation << i;
  i.close();

  loader->save();

  json fileToTestAfterOperation = { };
  i.open(pathToMainFile);
  fileToTestAfterOperation << i;
  i.close();

  ASSERT_EQ(fileToTestBeforeOperation.dump(4), fileToTestAfterOperation.dump(4));
}

TEST_F(TestRegularSingle, Open_file_while_already_loaded) {
  loader = LoaderPtr(FileSetFactory::createFileSet(pathToMainFile));
  loader->open();
  ASSERT_THROW(loader->open(), FileSetRuntimeError);
}

TEST_F(TestRegularSingle, Get_path_to_root_node) {
  loader = LoaderPtr(FileSetFactory::createFileSet(pathToMainFile));
  loader->open();
  FileSetLoader::Path path = loader->getPathToNode();
  ASSERT_EQ(pathToMainFile, path);
}

TEST_F(TestRegularSingle, Get_path_to_root_node_for_not_loaded) {
  loader = LoaderPtr(FileSetFactory::createFileSet(pathToMainFile));
  ASSERT_THROW(loader->getPathToNode(), FileSetRuntimeError);
}

TEST_F(TestRegularSingle, Get_project_name) {
  loader = LoaderPtr(FileSetFactory::createFileSet(pathToMainFile));
  loader->open();
  EXPECT_EQ(projectNameRef, loader->getProjectName());
}

TEST_F(TestRegularSingle, Get_project_name_for_not_loaded) {
  loader = LoaderPtr(FileSetFactory::createFileSet(pathToMainFile));
  ASSERT_THROW(loader->getProjectName(), FileSetRuntimeError);
}

TEST_F(TestRegularSingle, Get_list_of_files) {
  loader = LoaderPtr(FileSetFactory::createFileSet(pathToMainFile));
  loader->open();
  EXPECT_EQ(loader->getFileNames(), projectFilesRef);
}

TEST_F(TestRegularSingle, Get_list_of_files_for_not_loaded) {
  loader = LoaderPtr(FileSetFactory::createFileSet(pathToMainFile));
  ASSERT_THROW(loader->getFileNames(), FileSetRuntimeError);
}

TEST_F(TestRegularSingle, Count_subprojects_of_single_project) {
    loader = LoaderPtr(FileSetFactory::createFileSet(pathToMainFile));
    loader->open();
    ASSERT_EQ(0, loader->countSubprojects());
}

TEST_F(TestRegularSingle, Count_subprojects_of_sinlge_not_loaded) {
    loader = LoaderPtr(FileSetFactory::createFileSet(pathToMainFile));
    ASSERT_THROW(loader->countSubprojects(), FileSetRuntimeError);
}





class TestRegularNested : public TestRegular {
public:
  int subprojectsCount = 2;
  FileSetLoader::Subprojects subprojectsFilesRef = {"sub/s1.libproject", "sub/s2.libproject"};
  vector<string> subprojectsNamesRef = {"subpr1", "subpr2"};
  FileSetLoader::Path dirPath = {"project_files/nested/"};
  void SetUp() {
    pathToMainFile = {"project_files/nested/normal.libproject"};
    projectNameRef = {"Nested *** 43"};
    projectFilesRef = {"main.cpp", "Test.h", "Test.cpp"};
    TestRegular::SetUp();
  }
  void TearDown() {
      TestRegular::TearDown();
  }
};

TEST_F(TestRegularNested, Open_file) {
  loader = LoaderPtr(FileSetFactory::createFileSet(pathToMainFile));
  ASSERT_NO_THROW(loader->open());
}

TEST_F(TestRegularNested, Get_ACCESS_to_subprojects_if_not_loaded) {
    loader = LoaderPtr(FileSetFactory::createFileSet(pathToMainFile));
    map<string, FileSetLoader *> msl;
    ASSERT_THROW(msl = loader->getSubprojectLoaders(), FileSetRuntimeError);
}

TEST_F(TestRegularNested, Get_project_name_for_1st_subproject) {
    loader = LoaderPtr(FileSetFactory::createFileSet(pathToMainFile));
    loader->open();
    map<string, FileSetLoader *> msl = loader->getSubprojectLoaders();
    map<string, FileSetLoader *>::const_iterator it = msl.cbegin();
    string name = (*it).second->getProjectName();
    ASSERT_EQ(subprojectsNamesRef.at(0), name);
}

TEST_F(TestRegularNested, Count_subprojects_of_nested_project) {
    loader = LoaderPtr(FileSetFactory::createFileSet(pathToMainFile));
    loader->open();
    ASSERT_EQ(subprojectsCount, loader->countSubprojects());
}

TEST_F(TestRegularNested, Get_path_to_1st_subnode) {
    loader = LoaderPtr(FileSetFactory::createFileSet(pathToMainFile));
    loader->open();
    map<string, FileSetLoader *> msl = loader->getSubprojectLoaders();
    map<string, FileSetLoader *>::const_iterator it = msl.cbegin();
    FileSetLoader::Path path = (*it).second->getPathToNode();
    ASSERT_EQ(dirPath + subprojectsFilesRef.at(0), path);
}

TEST_F(TestRegularNested, Get_path_to_2n_subnode) {
   loader = LoaderPtr(FileSetFactory::createFileSet(pathToMainFile));
    loader->open();
    map<string, FileSetLoader *> msl = loader->getSubprojectLoaders();
    map<string, FileSetLoader *>::const_reverse_iterator it = msl.crbegin();
    //crbegin because we have only 2 item in map
    FileSetLoader::Path path = (*it).second->getPathToNode();
    ASSERT_EQ(dirPath + subprojectsFilesRef.at(1), path);
}





class TestAbnormalSingles : public TestSkeleton,
                          public ::testing::WithParamInterface<FileSetLoader::Path> {
public:
    void SetUp() {
        TestSkeleton::SetUp();
    }
    void TearDown() {
        TestSkeleton::TearDown();
    }
};

INSTANTIATE_TEST_CASE_P(InstantiationOfTestAbnormalSingles, TestAbnormalSingles,
                        ::testing::ValuesIn(pathsToSingleAbnormal));
TEST_P(TestAbnormalSingles, Set_of_attempts_to_open_abnormal_files) {
  ASSERT_THROW({
                 loader = LoaderPtr(FileSetFactory::createFileSet(GetParam()));
                 loader->open();
               },
               std::exception);
}





class TestAddRegularSubprojectsToSingle : public TestAddSubprojects {
public:
    FileSetLoader::Path pathToMainFileWhichIsTargetForAddBrokenSubproject = R"(project_files/testaddtosingle/mainproject_addbroken.libproject)";
    FileSetLoader::Path pathToOneRegularSingleSubproject = R"(project_files/testaddtosingle/regular/normalsingle.libproject)";
    FileSetLoader::Path pathToOneRegularNestedSubproject = R"(project_files/testaddtosingle/regular/normalnested.libproject)";
    FileSetLoader::Path pathToBrokenSubproject = R"(project_files/testaddtosingle/broken/broken.libproject)";
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
        TestAddSubprojects::SetUp();
    }
    void TearDown() {
        TestAddSubprojects::TearDown();
    }
};

TEST_F(TestAddRegularSubprojectsToSingle, Add_one_regular_single_subproject) {
    ASSERT_NO_THROW({
                        loader = LoaderPtr(FileSetFactory::createFileSet(pathToMainFile));
                        loader->open();
                        loader->addSubproject(pathToOneRegularSingleSubproject);
                        loader->save();
                        ifstream i(pathToMainFile);
                        fileToTest << i;
                 });
    ASSERT_EQ(contentReferenceWithSingle.dump(4), fileToTest.dump(4));
}


TEST_F(TestAddRegularSubprojectsToSingle, Add_one_regular_single_which_already_cached) {
    ASSERT_THROW({
                        loader = LoaderPtr(FileSetFactory::createFileSet(pathToMainFile));
                        loader->open();
                        loader->addSubproject(pathToOneRegularSingleSubproject);
                        loader->addSubproject(pathToOneRegularSingleSubproject);
                 }, FileSetRuntimeError);
    loader->save();
    ifstream i(pathToMainFile);
    fileToTest << i;

    ASSERT_EQ(contentReferenceWithSingle.dump(4), fileToTest.dump(4));
}





class TestAddRegularSubprojectsToNested : public TestAddSubprojects {
public:
    FileSetLoader::Path pathToPresentSubproject = R"(project_files/testaddtonested/presentsubproject/sub.libproject)";
    json contentReferenceWithPairOfNewRegularSubprojects = {
        { "project", "there must be subprojects" },
        { "files", { "main.cpp", "Test.h" } },
        { "subprojects", {"presentsubproject/sub.libproject",
                          "fortestwithtworegularsubprojects/nested.libproject",
                          "fortestwithtworegularsubprojects/single.libproject"} }
    };
    FileSetLoader::Path pathToOneRegularSingleSubproject = R"(project_files/testaddtonested/regular/normalsingle.libproject)";
    FileSetLoader::Path pathToOneRegularNestedSubproject = R"(project_files/testaddtonested/regular/normalnested.libproject)";
    FileSetLoader::Subprojects pathsToPairOfRegularSubprojects = {
        "project_files/testaddtonested/fortestwithtworegularsubprojects/single.libproject",
        "project_files/testaddtonested/fortestwithtworegularsubprojects/nested.libproject"
    };
    void SetUp() {
        pathToMainFile = pathToNestedMainFile;
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
        contentReference = {
          { "project", "there must be subprojects" },
          { "files", { "main.cpp", "Test.h" } },
          { "subprojects", {"presentsubproject/sub.libproject"} }
        };

        TestAddSubprojects::SetUp();
    }
    void TearDown() {
      TestAddSubprojects::TearDown();
    }
};

TEST_F(TestAddRegularSubprojectsToNested, Add_one_regular_single_subproject) {
    ASSERT_NO_THROW({
                        loader = LoaderPtr(FileSetFactory::createFileSet(pathToMainFile));
                        loader->open();
                        loader->addSubproject(pathToOneRegularSingleSubproject);
                        loader->save();
                        ifstream i(pathToMainFile);
                        fileToTest << i;
                 });

    ASSERT_EQ(contentReferenceWithSingle.dump(4), fileToTest.dump(4));
}

TEST_F(TestAddRegularSubprojectsToNested, Add_one_regular_nested_subproject) {
    ASSERT_NO_THROW({
                        loader = LoaderPtr(FileSetFactory::createFileSet(pathToMainFile));
                        loader->open();
                        loader->addSubproject(pathToOneRegularNestedSubproject);
                        loader->save();
                        ifstream i(pathToMainFile);
                        fileToTest << i;
                 });
    ASSERT_EQ(contentReferenceWithNested.dump(4), fileToTest.dump(4));
}

TEST_F(TestAddRegularSubprojectsToNested, Add_one_regular_single_which_already_cached) {
    ASSERT_THROW({
                        loader = LoaderPtr(FileSetFactory::createFileSet(pathToMainFile));
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
                        loader = LoaderPtr(FileSetFactory::createFileSet(pathToMainFile));
                        loader->open();
                        loader->addSubproject(pathToOneRegularSingleSubproject);
                        loader->save();
                        loader->addSubproject(pathToOneRegularSingleSubproject);
                    }, FileSetRuntimeError);
    loader->save();
    ifstream i(pathToMainFile);
    fileToTest << i;

    ASSERT_EQ(contentReferenceWithSingle.dump(4), fileToTest.dump(4));
}

TEST_F(TestAddRegularSubprojectsToNested, Add_already_cached_and_present_subprojects) {
    FileSetLoader::Subprojects subprojectsToAdd = {pathToOneRegularNestedSubproject, pathToPresentSubproject};

    ASSERT_NO_THROW({
                        loader = LoaderPtr(FileSetFactory::createFileSet(pathToMainFile));
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

    ASSERT_EQ(contentReferenceWithNested.dump(4), fileToTest.dump(4));
}

TEST_F(TestAddRegularSubprojectsToNested, Add_two_regular_subprojects) {
    ASSERT_NO_THROW({
                        loader = LoaderPtr(FileSetFactory::createFileSet(pathToMainFile));
                        loader->open();
                        loader->addSubprojects(pathsToPairOfRegularSubprojects);
                        loader->save();
                        ifstream i(pathToMainFile);
                        fileToTest << i;
                 });
    ASSERT_EQ(contentReferenceWithPairOfNewRegularSubprojects.dump(4), fileToTest.dump(4));
}





class TestAddAbnormalSubprojects : public TestAddSubprojects,
                          public ::testing::WithParamInterface<FileSetLoader::Subprojects> {
public:
  void SetUp() {
      contentReference = {
        { "project", "there must be subprojects" },
        { "files", { "main.cpp", "Test.h" } },
        { "subprojects", {"presentsubproject/sub.libproject"} }
      };
      pathToMainFile = pathToNestedMainFile;
      TestAddSubprojects::SetUp();
  }
  void TearDown() {
      TestAddSubprojects::TearDown();
  }
};

INSTANTIATE_TEST_CASE_P(InstantiationOfTestAbnormalSubprojects, TestAddAbnormalSubprojects,
                        ::testing::ValuesIn(pathsToAbnormalSubprojects));
TEST_P(TestAddAbnormalSubprojects, Set_of_attempts_to_add_abnormal_subprojects) {
    ASSERT_THROW({
                     loader = LoaderPtr(FileSetFactory::createFileSet(pathToMainFile));
                     loader->open();
                     loader->addSubprojects(GetParam());
                 },
                 FileSetRuntimeError);
        loader->save();
        ifstream i(pathToMainFile);
        fileToTest << i;

        ASSERT_EQ(contentReference.dump(4), fileToTest.dump(4));
}





class TestGetSubprojectsPaths : public TestSkeleton {
public:

    FileSetLoader::Path pathToProjectWithOneNormalSubprojects = R"(project_files/getsubprojectspaths/getpath.libproject)";
    FileSetLoader::Path pathToProjectWithPairNormalSubprojects = R"(project_files/getsubprojectspaths/getpaths.libproject)";
    FileSetLoader::Path pathToProjectWithoutSubprojects = R"(project_files/getsubprojectspaths/nosubprojects.libproject)";
    FileSetLoader::Subprojects pairOfPathsReference = {
        "sub/s1.libproject",
        "sub/s2.libproject"
    };
    FileSetLoader::Subprojects pathReference = {
        "sub/s1.libproject"
    };
    void SetUp() {
        TestSkeleton::SetUp();
    }
    void TearDown() {
        TestSkeleton::TearDown();
    }
};

TEST_F(TestGetSubprojectsPaths, Get_paths_of_lone_subproject) {
    loader = LoaderPtr(FileSetFactory::createFileSet(pathToProjectWithOneNormalSubprojects));
    loader->open();
    FileSetLoader::Subprojects paths = loader->getSubprojectsPaths();

    ASSERT_EQ(pathReference, paths);
}

TEST_F(TestGetSubprojectsPaths, Get_paths_of_two_subprojects) {
    loader = LoaderPtr(FileSetFactory::createFileSet(pathToProjectWithPairNormalSubprojects));
    loader->open();
    FileSetLoader::Subprojects paths = loader->getSubprojectsPaths();

    ASSERT_EQ(pairOfPathsReference, paths);
}

TEST_F(TestGetSubprojectsPaths, Get_subprojects_paths_of_project_without_them) {
    ASSERT_THROW ({
                        loader = LoaderPtr(FileSetFactory::createFileSet(pathToProjectWithoutSubprojects));
                        loader->open();
                        FileSetLoader::Subprojects paths = loader->getSubprojectsPaths();
                  },
                  FileSetRuntimeError);
}

TEST_F(TestGetSubprojectsPaths, Get_paths_of_subprojects_on_not_loaded_interface) {
    ASSERT_THROW ({
                      loader = LoaderPtr(FileSetFactory::createFileSet(pathToProjectWithPairNormalSubprojects));
                      FileSetLoader::Subprojects paths = loader->getSubprojectsPaths();
                  },
                  FileSetRuntimeError);
}



class TestRemoveSubprojects : public TestProjectsWithFewSubprojects {
public:
    FileSetLoader::Path pathToMainFileWhereNeedToRemoveOneSubprojectInArrayOfTwo =
            R"(project_files/testremove/case_0.libproject)";
    FileSetLoader::Path pathToMainFileWhereIsOneSubprojectWithWillBeRemoved =
            R"(project_files/testremove/case_4.libproject)";
    FileSetLoader::Path pathToMainFileWhichWillNotBeLoaded =
            R"(project_files/testremove/case_6.libproject)";
    FileSetLoader::Path pathToMainFileWhereWillBeRemoveAndCountActions =
            R"(project_files/testremove/case_7.libproject)";
    string contentBackup_oneSubproject;
    string contentBackup_withoutSubprojects;
    FileSetLoader::Path pathToPresentSubproject;
    FileSetLoader::Path pathToNonPresentSubproject;
    FileSetLoader::Subprojects pathsToNonPresentSubprojects;
    FileSetLoader::Subprojects pathsDuplicated;
    FileSetLoader::Subprojects pathsToPresentAndNonPresentSubprojects;
    void SetUp() {
        writableMainFile = true;
        pathToPresentSubproject = "sub/s1.libproject";

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

        TestProjectsWithFewSubprojects::SetUp();
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

      TestProjectsWithFewSubprojects::TearDown();
    }
};

TEST_F(TestRemoveSubprojects, Remove_one_subproject_from_file_with_two) {
    ASSERT_NO_THROW({
                        loader = LoaderPtr(FileSetFactory::createFileSet(pathToMainFileWhereNeedToRemoveOneSubprojectInArrayOfTwo));
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
                        loader = LoaderPtr(FileSetFactory::createFileSet(pathToMainFileWhereIsOneSubprojectWithWillBeRemoved));
                        loader->open();
                        loader->removeSubproject(pathToPresentSubproject);
                        loader->save();
                        ifstream i(pathToMainFileWhereIsOneSubprojectWithWillBeRemoved);
                        fileToTest << i;
                    });
    ASSERT_EQ(contentReference_withoutSuprojects.dump(4), fileToTest.dump(4));
}

TEST_F(TestRemoveSubprojects, Remove_on_not_loaded_interface) {
    ASSERT_THROW({
                        loader = LoaderPtr(FileSetFactory::createFileSet(pathToMainFileWhichWillNotBeLoaded));
                        loader->removeSubproject(pathToPresentSubproject);
                 }, FileSetRuntimeError);
    loader->save();
    ifstream i(pathToMainFileWhichWillNotBeLoaded);
    fileToTest << i;

    ASSERT_EQ(contentReference_withTwoSuprojects.dump(4), fileToTest.dump(4));
}





class TestIncorrectSubprojectsRemoving : public TestProjectsWithFewSubprojects,
        public ::testing::WithParamInterface<FileSetLoader::Subprojects> {
public:
    void SetUp() {

        writableMainFile = true;
        pathToMainFile = R"(project_files/testremove/case_1.libproject)";
        TestProjectsWithFewSubprojects::SetUp();
    }

    void TearDown() {
        TestProjectsWithFewSubprojects::TearDown();
    }
};

INSTANTIATE_TEST_CASE_P(InstantiationOfTestIncorrectSubprojectsRemoving, TestIncorrectSubprojectsRemoving,
                        ::testing::ValuesIn(incorrectPathsOfSubprojects));
TEST_P(TestIncorrectSubprojectsRemoving, Set_of_attempts_to_remove_subprojects_by_incorrect_paths) {
  ASSERT_THROW({
                 loader = LoaderPtr(FileSetFactory::createFileSet(pathToMainFile));
                 loader->open();
                 loader->removeSubprojects(GetParam());
    }, FileSetRuntimeError);

  loader->save();
  ifstream i(pathToMainFile);
  fileToTest << i;

  ASSERT_EQ(contentReference_withTwoSuprojects.dump(4), fileToTest.dump(4));
};






//TODO Count_subprojects_after_removing
//TODO Add tests for function getSubprojectsPaths() !!!!!!!!!!!!
//TODO Add tests for findSubprojectByPath() !!!!!!!!!!!!

} // namespace FileSetTests

int main(int argc, char **argv) {
  using namespace FileSetTests;
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
