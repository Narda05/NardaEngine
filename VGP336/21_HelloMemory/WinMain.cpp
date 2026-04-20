#include <NardaEngine/Inc/SpEngine.h>

using namespace NardaEngine;
using namespace NardaEngine::Core;

static int sUniqueId = 0;
static int sUniqueIdSubDirectors = 0;
static int sUniqueIdClasses = 0;
class Student
{
public:
	Student()
	{
		mId = ++sUniqueId;
		mMark = 60 + (rand() % 41);
		mName = " NoNmae_" + std::to_string(mId);
	}
	Student(const std::string& name)
		: mName(name)
	{
		mId = ++sUniqueId;
		mMark = 60 + (rand() % 41);
	}
	Student(const std::string& name, int mark)
		: mName(name)
		, mMark(mark)
	{
		mId = ++sUniqueId;
	}

	void Log()
	{
		LOG("Name: %s - ID: %d - Mark: %d", mName.c_str(), mId, mMark);
	}
private: 
	int mId = 0;
	int mMark = 0;
	std::string mName;

};


class Subdirectors {
public:
	Subdirectors()
	{
		mIdSubDirectors = ++sUniqueIdSubDirectors;
		mNumStudents = 60 + (rand() % 41);
		mName = " NoNmae_" + std::to_string(mIdSubDirectors);
	}

	void Log()
	{
		LOG("Name: %s - ID: %d - Mark: %d", mName.c_str(), mIdSubDirectors, mNumStudents);
	}
private:
	int mIdSubDirectors = 0;
	int mNumStudents = 0;
	std::string mName;

};

class Classes {
public:
	Classes()
	{
		mIdClasses = ++sUniqueIdClasses;
		mNumClasses = 60 + (rand() % 41);
		mName = " NoNmae_" + std::to_string(mIdClasses);
	}

	void Log()
	{
		LOG("Name: %s - ID: %d - Mark: %d", mName.c_str(), mIdClasses, mNumClasses);
	}
private:
	int mIdClasses = 0;
	int mNumClasses = 0;
	std::string mName;

};

int WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	TypedAllocator studentPool = TypedAllocator<Student>("StudentPool", 100);
	TypedAllocator subDirectorPool = TypedAllocator<Subdirectors>("SubDirectorPool", 80);
	TypedAllocator classPool = TypedAllocator<Classes>("ClassPool", 80);
	
	std::vector<Student*> students;
	std::vector<Subdirectors*> subDirectors;
	std::vector<Classes*> classes;

	// *STUDENTS test new
	for (uint32_t i = 0; i < 70; ++i)
	{
		std::string newStudentName = " NewStudent" + std::to_string(i);
		Student* newstudent = studentPool.New(newStudentName, 75);
		newstudent->Log();
		students.push_back(newstudent);

	}

	// SUBDIRECTORS test new
	for (uint32_t i = 0; i < 70; ++i)
	{
		Subdirectors* newSubDirector = subDirectorPool.New();
		newSubDirector->Log();
		subDirectors.push_back(newSubDirector);

	}
	
	// CLASSES test new
	for (uint32_t i = 0; i < 70; ++i)
	{
		Classes* newClass = classPool.New();
		newClass->Log();
		classes.push_back(newClass);
	}

	// *STUDENTS test delete
	for (uint32_t i = 0; i < 40; ++i)
	{
		Student* student = students.back();
		studentPool.Delete(student);
		students.pop_back();
	}

	// SUBDIRECTORS test delete
	for (uint32_t i = 0; i < 40; ++i)
	{
		Subdirectors* subDirector = subDirectors.back();
		subDirectorPool.Delete(subDirector);
		subDirectors.pop_back();
	}

	// CLASSES test delete
	for (uint32_t i = 0; i < 40; ++i)
	{
		Classes* classObj = classes.back();
		classPool.Delete(classObj);
		classes.pop_back();
	}

	// STUDENTS test add more 
	for (uint32_t i = 0; i < 50; ++i)
	{
		Student* newStudent = studentPool.New();
		newStudent->Log(); 
		students.push_back(newStudent);
	}

	// SUBDIRECTORS test add more

	for (uint32_t i = 0; i < 50; ++i)
	{
		Subdirectors* newSubDirector = subDirectorPool.New();
		newSubDirector->Log();
		subDirectors.push_back(newSubDirector);
	}

	// CLASSES test add more

	for (uint32_t i = 0; i < 50; ++i)
	{
		Classes* newClass = classPool.New();
		newClass->Log();
		classes.push_back(newClass);
	}


	// STUDENTS cleanup
	for (Student* student : students)
	{
		student->Log();
		studentPool.Delete(student);
	}
	students.clear();

	// SUBDIRECTORS cleanup

	for (Subdirectors* subDirector : subDirectors)
	{
		subDirector->Log();
		subDirectorPool.Delete(subDirector);
	}
	subDirectors.clear();

	// CLASSES cleanup

	for (Classes* classObj : classes)
	{
		classObj->Log();
		classPool.Delete(classObj);
	}
	classes.clear();

	return 0;

}