#include <Windows.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <time.h>
#include <cmath>
#include <cstdlib>
using namespace std;


#define EXAMS_COUNT 3
#define TESTS_COUNT 5
#define MAX_NAME_SIZE 10
#define TIMESTAMP_SIZE 20
#define STUDENT_SIZE 7
#define MAX_DATA_SIZE max(MAX_NAME_SIZE, TIMESTAMP_SIZE) // максимальная длина строки в файле students.txt


void getCurrentTime(char* formatTime) {
  struct tm * timeinfo;
  time_t tstamp;
  time(&tstamp);
  timeinfo = localtime(&tstamp);
  strftime(formatTime, TIMESTAMP_SIZE, "%Y-%m-%d.%X", timeinfo);
}

int getRandomValueFromRange(int limit) {
  return rand() % (limit + 1);
}


struct Student {
  unsigned int id;
  char name[MAX_NAME_SIZE];
  char gender;
  unsigned int groupNumber;
  short unsigned int groupListNumber;
  short unsigned int grades[EXAMS_COUNT + TESTS_COUNT];
  float avgScore;
  char timestamp[TIMESTAMP_SIZE];

  void calculateAverageScore() {
    this->avgScore = 0.0f;
    for (int i = 0; i < EXAMS_COUNT + TESTS_COUNT; i++) {
      avgScore += this->grades[i];
    }
    this->avgScore = round((this->avgScore / (EXAMS_COUNT + TESTS_COUNT)) * 100.f) / 100.f;
  }
};

int countStudents() {
	ifstream database("students.txt");
	if (database.is_open()) {
		int temp = 0;
		char data[MAX_DATA_SIZE];
		while (database.getline(data, MAX_DATA_SIZE))
		{
			temp++;
		}
		database.close();
		int n;
		n = temp / STUDENT_SIZE;
		return n;
	}
	return 0;
}

void showStudent(Student &student) {
  cout << "\nStudent id: " << student.id << "\n"
  << "  name: " << student.name << "\n"
  << "  gender: " << student.gender << "\n"
  << "  groupNumber: " << student.groupNumber << "\n"
  << "  groupListNumber: " << student.groupListNumber << "\n"
  << "  grades: ";
  for (int i = 0; i < (EXAMS_COUNT + TESTS_COUNT); i++) cout << student.grades[i] << " ";
  cout << "avg: " << student.avgScore << "\n"
  << "  timestamp: " << student.timestamp << "\n";
}

void bubbleSort(Student arr[], const int N) {
    for (int i = 0; i < N - 1; i++) {
        for (int j = 0; j < N - i - 1; j++) {
            struct Student swap = arr[j];
            if (arr[j].avgScore < arr[j + 1].avgScore) {
                arr[j] = arr[j + 1];
                arr[j + 1] = swap;
            }
        }
    }     
}

void showSortedStudents(Student students[], const int studentsCount) {
  bubbleSort(students, studentsCount);
  for (int i = 0; i < studentsCount; i++) showStudent(students[i]);
}

void readData(Student students[], const int studentsCount) {
  char data[MAX_DATA_SIZE];
  ifstream db("students.txt");
  int idx = 0;
  short unsigned int fieldType = 0;
  while(db.getline(data, MAX_DATA_SIZE)) {
    switch (fieldType)
      {
        case 0: {
          students[idx].id = atoi(data);
          break;
        }
        case 1: {
          for (int i = 0; i < MAX_NAME_SIZE; i++) {
            students[idx].name[i] = data[i];
          }
          break;
        }
        case 2: {
          students[idx].gender = data[0];
          break;
        }
        case 3: {
          students[idx].groupNumber = atoi(data);
          break;
        }
        case 4: {
          students[idx].groupListNumber = atoi(data);
          break;
        }
        case 5: {
          for (int i = 0; i < (EXAMS_COUNT + TESTS_COUNT); i++) {
            students[idx].grades[i] = static_cast<int>(data[i] - '0');
            students[idx].calculateAverageScore();
          }
          break;
        }
        case 6: {
          for (int i = 0; i < TIMESTAMP_SIZE; i++) {
            students[idx].timestamp[i] = data[i];
          }
          break;
        }
      }
    fieldType++;
    if (fieldType == STUDENT_SIZE) {
      showStudent(students[idx]);
      fieldType = 0;
      idx++;
    }
  }
  db.close();
}

void clearStudentsFile() {
  ofstream db("students.txt");
  if (!db.is_open()) {
    cerr << "Error opening students file for writing" << endl;
    return;
  }
  db << "";
  db.close();
}

void writeData(Student& student) {
  ofstream database("students.txt", ios::app); // начинаем запись с последней строки файла
	if (!database.is_open()) {
	  cout << '\n' << "Saving error!";
	} else {
    char charGrades[EXAMS_COUNT + TESTS_COUNT];
    char buffer[sizeof(unsigned short int)];
    for (int i = 0; i < EXAMS_COUNT + TESTS_COUNT; i++) charGrades[i] = *itoa(student.grades[i], buffer, 10);
    student.calculateAverageScore();    
		database 
    << student.id << '\n'
    << student.name << '\n'
    << student.gender << '\n'
    << student.groupNumber << '\n'
    << student.groupListNumber << '\n';
    for (int i = 0; i < EXAMS_COUNT + TESTS_COUNT; i++) database << charGrades[i];
		database << '\n'
    << student.timestamp
    << '\n';
		database.close();
	}
}


Student createStudent() {
  struct Student student;
  cout << "\nEnter a data:\n";
  cout << "name (max " << MAX_NAME_SIZE << " symbols): ";
  // cin >> student.name;
  cin.get();
  cin.getline(student.name, MAX_NAME_SIZE);
  cin.clear(); // Clearing the input stream from possible errors
  cin.sync();
  cout << "gender (char): ";
  cin >> student.gender;
  cout << "groupNumber: ";
  cin >> student.groupNumber;
  cout << "groupListNumber: ";
  cin >> student.groupListNumber;
  cout << "grades (" << (EXAMS_COUNT + TESTS_COUNT) <<" numbers): ";
  for (int i = 0; i < (EXAMS_COUNT + TESTS_COUNT); i++) cin >> student.grades[i];
  
  student.id = getRandomValueFromRange(1000);

  char currentTime[TIMESTAMP_SIZE];
  getCurrentTime(currentTime);
  for (int i = 0; i < TIMESTAMP_SIZE; i++) student.timestamp[i] = currentTime[i];

  writeData(student);
  return student;
}

int getIndexOfStudent(unsigned int id, Student *students, const int N) {
  for (int i = 0 ; i < N; i++) {
    if (students[i].id == id) return i;
  }
  return -1;
}

void updateStudent(struct Student* student, const short unsigned int fieldType) {
  switch (fieldType)
  {
    case 1: {
      cin.get();
      cin.getline(student->name, MAX_NAME_SIZE);
      cin.clear(); // Clearing the input stream from possible errors
      cin.sync();
      break;
    }
    case 2: {
      cin >> student->gender;
      break;
    }
    case 3: {
      cin >> student->groupNumber;
      break;
    }
    case 4: {
      cin >> student->groupListNumber;
      break;
    }
    case 5: {
      for (int i = 0; i < (EXAMS_COUNT + TESTS_COUNT); i++) cin >> student->grades[i];
      student->calculateAverageScore();
      break;
    }
  }
}


int main() {
  // If when entering RU characters there are hieroglyphs, then write the command in the console: chcp 1251
  SetConsoleCP(1251); // Setting the input encoding
  SetConsoleOutputCP(1251); // Setting the output encoding
  setlocale(LC_ALL, "Russian");
  srand(time(0));

  cout << "Navigation:" << "\n"
  << "1) Create a new student record" << "\n"
  << "2) Making changes to an existing record" << "\n"
  << "3) Displaying all student data" << "\n"
  << "4) Displays info about all students in group <N>" << "\n"
  << "5) Finding top students with the highest rated average score for the last session" << "\n"
  << "6) Displaying the number of male and female students" << "\n"
  << "7) Displaying students who don't receive a scholarship; who have only 4 or 5 grades; who have only 5 grades" << "\n"
  << "8) Displaying data about students with list number <k>" << "\n";
  
  int fileStudentsCount = countStudents();
  short unsigned int newStudentsCount = 0; // те, кого ещё нет в файле, но хотим добавить
  cout << "\nStudents count in file: "<< fileStudentsCount << "\n";
  cout << "Enter number of new students: ";
  cin >> newStudentsCount;
  if (!cin.good()) {
    cout << "\nYou entered an incorrect value\n";
  }
  int aviablePlaces = newStudentsCount; // тк работаем без векторов, то заранее указываем сколько новых
  const int studentsCount = fileStudentsCount + newStudentsCount;

  struct Student students[studentsCount];
  readData(students, studentsCount);
  cout << "\n^^^ Students data from file ^^^\n";

  while(true) {
    cin.clear(); // Clearing the input stream from possible errors
    cin.sync();
    short int workPoint;
        
    cout << "Select point of work (number 1 to 8): ";
    cin >> workPoint;

    switch (workPoint)
    {   
      case 1: {
        
        break;
      }
      case 2: {
        
        break;
      }
      case 3: {
        
        break;
      }
      case 4: {
        
        break;
      }
      case 5: {
        
        break;
      }
      case 6: {
        
        break;
      }
      case 7:{
        
        break;
      }
      case 8: {
        
        break;
      }
      default: {
        cout << "\n" << "You did not enter a number in the range from 1 to 8";
        break;
      }
    }

    cin.clear(); // Clearing the input stream from possible errors
    cin.sync();

    char stopFlag;
    cout << "\n" << "Continue the program? (Y/N) ";
    cin >> stopFlag;
        
    if (stopFlag != 'Y' && stopFlag != 'y') break;

    }
    
    return 0;
}