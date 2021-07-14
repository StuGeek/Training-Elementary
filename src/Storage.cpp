#include "../include/Storage.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <stack>
#include <vector>
#include "../include/Path.hpp"
#include <iostream>

using namespace std;

// instance of Storage
shared_ptr<Storage> Storage::m_instance = nullptr;

/**
 *   default constructor
 */
Storage::Storage() {
  m_dirty = false;
  readFromFile();
}

vector<string> removeSymbolAnd(string &t_participator) {
  vector<string> participator;
  string select = "";
  for (int i = 0; i < t_participator.size(); ++i) {
    if (i == t_participator.size() - 1) {
      select += t_participator[i];
      participator.push_back(select);
    } else if (t_participator[i] == '&') {
      participator.push_back(select);
      select = "";
    } else
      select += t_participator[i];
  }
  return participator;
}

vector<string> lineToVector(string &oneLine) {
  vector<string> information;
  string select = "";
  stack<bool> quotation;
  for (int i = 0; i < oneLine.size(); ++i) {
    if (oneLine[i] == '\"') {
      if (quotation.empty())
        quotation.push(true);
      else {
        information.push_back(select);
        select = "";
        quotation.pop();
      }
    } else if (oneLine[i] == ',')
      continue;
    else {
      select += oneLine[i];
    }
  }
  return information;
}

/**
 *   read file content into memory
 *   @return if success, true will be returned
 */
bool Storage::readFromFile(void) {
  ifstream ifsOfUsers(Path::userPath);
  ifstream ifsOfMeeting(Path::meetingPath);
  if (ifsOfUsers.fail()) return false;
  if (ifsOfMeeting.fail()) return false;
  string oneLine = "";
  while (getline(ifsOfUsers, oneLine)) {
    if (oneLine.size() < 1) continue;
    vector<string> information = lineToVector(oneLine);
    if (information.size() != 4) continue;
    User t_user(information[0], information[1], information[2], information[3]);
    m_userList.push_back(t_user);
  }
  while (getline(ifsOfMeeting, oneLine)) {
    if (oneLine.length() < 1) continue;
    vector<string> information = lineToVector(oneLine);
    if (information.size() != 5) continue;
    vector<string> participators = removeSymbolAnd(information[1]);
    Meeting t_meeting(information[0], participators, information[2],
                      information[3], information[4]);
    m_meetingList.push_back(t_meeting);
    ifsOfUsers.close();
    ifsOfMeeting.close();
    return true;
  }
}

string addSymbolAnd(const vector<string> &t_participators) {
  string participators = "";
  for (int i = 0; i < t_participators.size(); ++i) {
    participators += t_participators[i];
    if (i != t_participators.size() - 1) participators += '&';
  }
  return participators;
}

string vectorToLine(const vector<string> &information) {
  string oneLine = "";
  for (int i = 0; i < information.size(); ++i) {
    oneLine += "\"" + information[i] + "\"";
    if (i != information.size() - 1) oneLine += ",";
  }
  return oneLine;
}
/**
 *   write file content from memory
 *   @return if success, true will be returned
 */
bool Storage::writeToFile(void) {
  if (!m_dirty) return false;
  ofstream ofsOfUsers(Path::userPath);
  ofstream ofsOfMeeting(Path::meetingPath);
  for (auto it = m_userList.begin(); it != m_userList.end(); it++) {
    vector<string> users_information = {(*it).getName(), (*it).getPassword(),
                                        (*it).getEmail(),
                                        (*it).getPhone()};
	ofsOfUsers << vectorToLine(users_information) << endl;
  }
  for (auto it = m_meetingList.begin(); it != m_meetingList.end(); it++) {
    string participators = addSymbolAnd((*it).getParticipator());
    string startDate = Date::dateToString((*it).getStartDate());
    string endDate = Date::dateToString((*it).getEndDate());
    vector<string> meetings_information = {(*it).getSponsor(), participators,
                                           startDate, endDate,
                                           (*it).getTitle()}; 
	ofsOfMeeting << vectorToLine(meetings_information) << endl;
  }
  m_dirty = false;
  return true;
}

/**
 * get Instance of storage
 * @return the pointer of the instance
 */
shared_ptr<Storage> Storage::getInstance(void) {
  if (m_instance == nullptr) {
    m_instance = shared_ptr<Storage>(new Storage);
  }
  return m_instance;
}

/**
 *   destructor
 */
Storage::~Storage() {
  // cout << "delete storage" << endl;
  if (m_dirty) writeToFile();
}

// CRUD for User & Meeting
// using C++11 Function Template and Lambda Expressions

/**
 * create a user
 * @param a user object
 */
void Storage::createUser(const User &t_user) {
  m_dirty = true;
  m_userList.push_back(t_user);
}

/**
 * query users
 * @param a lambda function as the filter
 * @return a list of fitted users
 */
list<User> Storage::queryUser(function<bool(const User &)> filter) const {
  list<User> t_userList;
  for (auto it = m_userList.begin(); it != m_userList.end(); it++) {
    if (filter(*it)) t_userList.push_back(*it);
  }
  return t_userList;
}

/**
 * update users
 * @param a lambda function as the filter
 * @param a lambda function as the method to update the user
 * @return the number of updated users
 */
int Storage::updateUser(function<bool(const User &)> filter,
                        function<void(User &)> switcher) {
  int numberOfUpdatedUsers = 0;
  for (auto it = m_userList.begin(); it != m_userList.end(); it++) {
    if (filter(*it)) {
      m_dirty = true;
      numberOfUpdatedUsers++;
      switcher(*it);
    }
  }
  return numberOfUpdatedUsers;
}

/**
 * delete users
 * @param a lambda function as the filter
 * @return the number of deleted users
 */
int Storage::deleteUser(function<bool(const User &)> filter) {
  int numberOfDeletedUsers = 0;
  for (auto it = m_userList.begin(); it != m_userList.end();) {
    if (filter(*it)) {
      m_dirty = true;
      numberOfDeletedUsers++;
      it = m_userList.erase(it);
    } else
      it++;
  }
  return numberOfDeletedUsers;
}

/**
 * create a meeting
 * @param a meeting object
 */
void Storage::createMeeting(const Meeting &t_meeting) {
  m_dirty = true;
  m_meetingList.push_back(t_meeting);
}

/**
 * query meetings
 * @param a lambda function as the filter
 * @return a list of fitted meetings
 */
list<Meeting> Storage::queryMeeting(
    function<bool(const Meeting &)> filter) const {
  list<Meeting> t_meetingList;
  for (auto it = m_meetingList.begin(); it != m_meetingList.end(); it++) {
    if (filter(*it)) t_meetingList.push_back(*it);
  }
  return t_meetingList;
}

/**
 * update meetings
 * @param a lambda function as the filter
 * @param a lambda function as the method to update the meeting
 * @return the number of updated meetings
 */
int Storage::updateMeeting(function<bool(const Meeting &)> filter,
                           function<void(Meeting &)> switcher) {
  int numberOfUpdatedMeetings = 0;
  for (auto it = m_meetingList.begin(); it != m_meetingList.end(); it++) {
    if (filter(*it)) {
      m_dirty = true;
      numberOfUpdatedMeetings++;
      switcher(*it);
    }
  }
  return numberOfUpdatedMeetings;
}

/**
 * delete meetings
 * @param a lambda function as the filter
 * @return the number of deleted meetings
 */
int Storage::deleteMeeting(function<bool(const Meeting &)> filter) {
  int numberOfDeletedMeetings = 0;
  for (auto it = m_meetingList.begin(); it != m_meetingList.end();) {
    if (filter(*it)) {
      m_dirty = true;
      numberOfDeletedMeetings++;
      it = m_meetingList.erase(it);
    } else
      it++;
  }
  return numberOfDeletedMeetings;
}

/**
 * sync with the file
 */
bool Storage::sync(void) { return writeToFile(); }