#include "../include/AgendaService.hpp"
#include <list>
#include <vector>
#include <string>
#include "../include/Date.hpp"
#include "../include/Meeting.hpp"
#include "../include/User.hpp"
#include <algorithm>
#include <iostream>

using namespace std;

AgendaService::AgendaService() { m_storage = Storage::getInstance(); }

AgendaService::~AgendaService() { m_storage->sync(); }

bool AgendaService::userLogIn(const std::string &userName,
                              const std::string &password) {
  auto user_filter = [&userName, &password](const User &user) {
    return userName == user.getName() && password == user.getPassword();
  };
  return m_storage->queryUser(user_filter).size() > 0;
}

bool AgendaService::userRegister(const std::string &userName,
                                 const std::string &password,
                                 const std::string &email,
                                 const std::string &phone) {
  auto user_filter = [&userName](const User &user) {
    return userName == user.getName();
  };
  if (m_storage->queryUser(user_filter).size() == 0) {
    User t_user(userName, password, email, phone);
    m_storage->createUser(t_user);
    return true;
  } else
    return false;
}

bool AgendaService::deleteUser(const std::string &userName,
                               const std::string &password) {
  auto user_filter = [&userName, &password](const User &user) {
    return userName == user.getName() && password == user.getPassword();
  };
  if (m_storage->queryUser(user_filter).size() == 0)
    return false;
  else {
    m_storage->deleteUser(
        [&userName](const User &user) { return userName == user.getName(); });
  }
  m_storage->updateMeeting(
      [&userName](const Meeting &meeting) {
        return meeting.isParticipator(userName);
      },
      [&userName](Meeting &meeting) { meeting.removeParticipator(userName); });
  m_storage->deleteMeeting([&userName](const Meeting &meeting) {
    return meeting.getSponsor() == userName ||
           meeting.getParticipator().size() == 0;
  });
  return true;
}

list<User> AgendaService::listAllUsers(void) const {
  list<User> t_userList =
      m_storage->queryUser([](const User &user) { return true; });
  return t_userList;
}

bool AgendaService::createMeeting(
    const std::string &userName, const std::string &title,
    const std::string &startDate, const std::string &endDate,
    const std::vector<std::string> &participator) {
  Date startdate = Date::stringToDate(startDate);
  Date enddate = Date::stringToDate(endDate);
  if (!Date::isValid(startdate) || !Date::isValid(enddate) ||
      startdate >= enddate)
    return false;

  for (int i = 0; i < participator.size(); ++i) {
    for (int j = i + 1; j < participator.size(); ++j) {
      if (participator[i] == participator[j]) return false;
    }
  }

  auto title_filter = [&title](const Meeting &meeting) {
    return meeting.getTitle() == title;
  };
  if (m_storage->queryMeeting(title_filter).size() != 0) return false;

  auto userExit_filter = [&userName](const User &user) {
    return user.getName() == userName;
  };
  if (m_storage->queryUser(userExit_filter).size() == 0) return false;

  auto sponserDate_filter = [&userName, &startdate,
                             &enddate](const Meeting &meeting) {
    if (meeting.isParticipator(userName) || meeting.getSponsor() == userName) {
      Date t_startdate = meeting.getStartDate();
      Date t_enddate = meeting.getEndDate();
      return (t_startdate >= startdate && t_enddate <= enddate) ||
             (t_startdate <= startdate && t_enddate > startdate) ||
             (t_startdate < enddate && t_enddate >= enddate);
    } else
      return false;
  };
  if (m_storage->queryMeeting(sponserDate_filter).size() != 0) return false;

  for (int i = 0; i < participator.size(); ++i) {
    if (userName == participator[i]) return false;
    string participatorName = participator[i];
    list<User> t_userList =
        m_storage->queryUser([&participatorName](const User &user) {
          return user.getName() == participatorName;
        });
    if (t_userList.size() == 0) return false;
    list<Meeting> t_meetingLists = m_storage->queryMeeting(
        [&participatorName, &startdate, &enddate](const Meeting &meeting) {
          if (meeting.isParticipator(participatorName) ||
              meeting.getSponsor() == participatorName) {
            Date t_startdate = meeting.getStartDate();
            Date t_enddate = meeting.getEndDate();
            return (t_startdate >= startdate && t_enddate <= enddate) ||
                   (t_startdate <= startdate && t_enddate > startdate) ||
                   (t_startdate < enddate && t_enddate >= enddate);
          } else
            return false;
        });
    if (t_meetingLists.size() != 0) return false;
  }

  Meeting t_meeting(userName, participator, startdate, enddate, title);
  m_storage->createMeeting(t_meeting);
  return true;
}

bool AgendaService::addMeetingParticipator(const std::string &userName,
                                           const std::string &title,
                                           const std::string &participator) {
  list<User> t_paticipatorLists =
      m_storage->queryUser([&participator](const User &user) {
        return participator == user.getName();
      });
  if (t_paticipatorLists.size() == 0) return false;
  list<Meeting> t_meetingLists =
      m_storage->queryMeeting([&userName, &title](const Meeting &meeting) {
        return userName == meeting.getSponsor() && title == meeting.getTitle();
      });
  if (t_meetingLists.size() == 0) return false;
  Meeting t_meeting = t_meetingLists.front();
  Date t_startdate = t_meeting.getStartDate();
  Date t_enddate = t_meeting.getEndDate();
  list<Meeting> t_dateLists = m_storage->queryMeeting(
      [&participator, &t_startdate, &t_enddate](const Meeting &meeting) {
        if (meeting.isParticipator(participator) ||
            meeting.getSponsor() == participator) {
          Date startdate = meeting.getStartDate();
          Date enddate = meeting.getEndDate();
          return (startdate >= t_startdate && enddate <= t_enddate) ||
                 (startdate <= t_startdate && enddate > t_startdate) ||
                 (startdate < t_enddate && enddate >= t_enddate);
        } else
          return false;
      });
  if (t_dateLists.size() != 0) return false;
  auto meeting_filter = [&t_meeting](const Meeting &meeting) {
    return meeting.getTitle() == t_meeting.getTitle();
  };
  auto meeting_switcher = [&participator](Meeting &meeting) {
    meeting.addParticipator(participator);
  };
  m_storage->updateMeeting(meeting_filter, meeting_switcher);
  return true;
}

bool AgendaService::removeMeetingParticipator(const std::string &userName,
                                              const std::string &title,
                                              const std::string &participator) {
  list<Meeting> t_participatorLists =
      m_storage->queryMeeting([&participator, &title](const Meeting &meeting) {
        return meeting.isParticipator(participator) && title == meeting.getTitle();
      });
  if (t_participatorLists.size() == 0) return false;

  list<Meeting> t_meetingLists =
      m_storage->queryMeeting([&userName, &title](const Meeting &meeting) {
        return userName == meeting.getSponsor() && title == meeting.getTitle();
      });
  if (t_meetingLists.size() == 0) return false;

  Meeting t_meeting = t_meetingLists.front();

  auto meeting_filter = [&t_meeting](const Meeting &meeting) {
    return meeting.getTitle() == t_meeting.getTitle();
  };
  auto meeting_switcher = [&participator](Meeting &meeting) {
    meeting.removeParticipator(participator);
  };
  m_storage->updateMeeting(meeting_filter, meeting_switcher);
  m_storage->deleteMeeting([&t_meeting](const Meeting & meeting){
        return meeting.getTitle() == t_meeting.getTitle() && meeting.getParticipator().size() == 0;
    });
  return true;
}

bool AgendaService::quitMeeting(const std::string &userName,
                                const std::string &title) {
  list<Meeting> t_meetingLists =
      m_storage->queryMeeting([&userName, &title](const Meeting &meeting) {
        return meeting.isParticipator(userName) && title == meeting.getTitle();
      });
  if (t_meetingLists.size() == 0) return false;
  Meeting t_meeting = t_meetingLists.front();
  auto meeting_filter = [&t_meeting](const Meeting &meeting) {
    return meeting.getTitle() == t_meeting.getTitle();
  };
  auto meeting_switcher = [&userName](Meeting &meeting) {
    meeting.removeParticipator(userName);
  };
  m_storage->updateMeeting(meeting_filter, meeting_switcher);
  m_storage->deleteMeeting([&t_meeting](const Meeting &meeting) {
    return meeting.getTitle() == t_meeting.getTitle() &&
           meeting.getParticipator().size() == 0;
  });
  return true;
}

std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                               const std::string &title) const {
  auto meeting_filter = [&userName, &title](const Meeting &meeting) {
    if (meeting.isParticipator(userName) || meeting.getSponsor() == userName) {
      return title == meeting.getTitle();
    } else
      return false;
  };
  list<Meeting> t_meetingLists = m_storage->queryMeeting(meeting_filter);
  return t_meetingLists;
}

std::list<Meeting> AgendaService::meetingQuery(
    const std::string &userName, const std::string &startDate,
    const std::string &endDate) const {
  Date startdate = Date::stringToDate(startDate);
  Date enddate = Date::stringToDate(endDate);
  if (!Date::isValid(startdate) || !Date::isValid(enddate) ||
      startdate > enddate)
    return list<Meeting>();
  auto meeting_filter = [&userName, &startdate,
                         &enddate](const Meeting &meeting) {
    if (meeting.isParticipator(userName) || meeting.getSponsor() == userName) {
      Date t_startdate = meeting.getStartDate();
      Date t_enddate = meeting.getEndDate();
      return (t_startdate >= startdate && t_enddate <= enddate) ||
             (t_startdate <= startdate && t_enddate >= startdate) ||
             (t_startdate <= enddate && t_enddate >= enddate);
    } else
      return false;
  };
  list<Meeting> t_meetingLists = m_storage->queryMeeting(meeting_filter);
  return t_meetingLists;
}

std::list<Meeting> AgendaService::listAllMeetings(
    const std::string &userName) const {
  list<Meeting> t_meetingLists =
      m_storage->queryMeeting([&userName](const Meeting &meeting) {
        return userName == meeting.getSponsor() ||
               meeting.isParticipator(userName);
      });
  return t_meetingLists;
}

std::list<Meeting> AgendaService::listAllSponsorMeetings(
    const std::string &userName) const {
  list<Meeting> t_meetingLists =
      m_storage->queryMeeting([&userName](const Meeting &meeting) {
        return userName == meeting.getSponsor();
      });
  return t_meetingLists;
}

std::list<Meeting> AgendaService::listAllParticipateMeetings(
    const std::string &userName) const {
  list<Meeting> t_meetingLists =
      m_storage->queryMeeting([&userName](const Meeting &meeting) {
        return meeting.isParticipator(userName);
      });
  return t_meetingLists;
}

bool AgendaService::deleteMeeting(const std::string &userName,
                                  const std::string &title) {
  auto meeting_filter = [&userName, &title](const Meeting &meeting) {
    return userName == meeting.getSponsor() && title == meeting.getTitle();
  };
  int numOfDeletedMeeting = m_storage->deleteMeeting(meeting_filter);
  return numOfDeletedMeeting > 0;
}

bool AgendaService::deleteAllMeetings(const std::string &userName) {
  auto meeting_filter = [&userName](const Meeting &meeting) {
    return userName == meeting.getSponsor();
  };
  int numOfDeletedMeeting = m_storage->deleteMeeting(meeting_filter);
  return numOfDeletedMeeting > 0;
}

void AgendaService::startAgenda(void) { m_storage = Storage::getInstance(); }

void AgendaService::quitAgenda(void) { m_storage->sync(); }
