#ifndef AGENDASERVICE_H
#define AGENDASERVICE_H

#include <list>
#include <string>
#include "../include/Storage.hpp"

using namespace std;

class AgendaService
{
public:
  AgendaService();
  ~AgendaService();
  bool userLogIn(const std::string &userName, const std::string &password);

  bool userRegister(const std::string &userName, const std::string &password,
                    const std::string &email, const std::string &phone);

  bool deleteUser(const std::string &userName, const std::string &password);

  list<User> listAllUsers(void) const;

  bool createMeeting(const std::string &userName, const std::string &title,
                     const std::string &startDate, const std::string &endDate,
                     const std::vector<std::string> &participator);

  bool addMeetingParticipator(const std::string &userName,
                              const std::string &title,
                              const std::string &participator);

  bool removeMeetingParticipator(const std::string &userName,
                                 const std::string &title,
                                 const std::string &participator);

  bool quitMeeting(const std::string &userName, const std::string &title);

  list<Meeting> meetingQuery(const std::string &userName,
                             const std::string &title) const;

  list<Meeting> meetingQuery(const std::string &userName,
                             const std::string &startDate,
                             const std::string &endDate) const;

  list<Meeting> listAllMeetings(const std::string &userName) const;

  list<Meeting> listAllSponsorMeetings(const std::string &userName) const;

  list<Meeting> listAllParticipateMeetings(
      const std::string &userName) const;

  bool deleteMeeting(const std::string &userName, const std::string &title);

  bool deleteAllMeetings(const std::string &userName);

  void startAgenda(void);

  void quitAgenda(void);

private:
  std::shared_ptr<Storage> m_storage;
};

#endif
