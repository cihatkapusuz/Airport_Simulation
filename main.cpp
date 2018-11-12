#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>
#include <functional>
#include <vector>
#include <queue>


using namespace std;

struct Passenger {

    int arrival_time;
    int boarding_time;
    int luggage_waiting;
    int security_waiting;
    string luggage;
    string vip;

    Passenger(){
        this->arrival_time = 0;
        this->boarding_time = 0;
        this->luggage_waiting = 0;
        this->security_waiting = 0;
        this->vip = "";
        this->luggage = "";
    }

    Passenger(int _arrival_time, int _boarding_time, int _luggage_waiting, int _security_waiting, string _vip,
              string _luggage) {

        this->arrival_time = _arrival_time;
        this->boarding_time = _boarding_time;
        this->luggage_waiting = _luggage_waiting;
        this->security_waiting = _security_waiting;
        this->vip = _vip;
        this->luggage = _luggage;
    }
    bool operator < (Passenger other) const{
        if(this->boarding_time!=other.boarding_time){
            return this->boarding_time < other.boarding_time;
        }
        else{
            return this->arrival_time < other.arrival_time;
        }

    }
};

struct Event {
    int time;
    int type;
    Passenger* p;
    Event(int _time, int _type, Passenger* _p ){
        this->time=_time;
        this->type=_type;
        this->p=_p;
    }
    bool operator < (Event other) const{
        if(this->time!=other.time){
            return time > other.time;
        }
        else{
            if(this->type!=other.type){
                return this->type < other.type;
            }
            else {
                return this->p->arrival_time > other.p->arrival_time;
            }
        }
    }

};
struct firstfly{
    bool operator()(Passenger* a, Passenger* b)
    {
        if(a->boarding_time!= b->boarding_time){
            return a->boarding_time > b->boarding_time;
        }
        else{
            return a->arrival_time > b->arrival_time;

        }
    }

};


int main(int argc, char* argv[]) {

    if (argc != 3) {
        return 1;
    }


    // here, perform the input operation. in other words,
    // read the file named <argv[1]>
    ifstream infile(argv[1]);
    string line;
    // process first line
    getline(infile, line);
    stringstream  stream(line);
    int n_of_passengers;
    stream >> n_of_passengers;
    int n_of_luggage_counters ;
    stream >> n_of_luggage_counters;
    int n_of_security_counters ;
    stream >> n_of_security_counters;
    vector<Passenger*> pass;
    int at;
    int bt;
    int lt;
    int st;
    string vi;
    string ot;
      for (int i=0; i<n_of_passengers; i++) {
          getline(infile, line);
          stringstream  stream(line);
          stream >> at;
          stream >> bt;
          stream >> lt;
          stream >> st;
          stream >> vi;
          stream >> ot;
          pass.push_back(new Passenger(at ,bt ,lt ,st ,vi ,ot));
    }
    priority_queue<Passenger*> ftf;
      for (auto pas : pass) {
          ftf.push(pas);
      }

    int time = 0;
    priority_queue<Event> event;
      int available_security = n_of_security_counters;
      int available_luggage = n_of_luggage_counters;
      for (auto &pas : pass) {
          event.push(Event(pas->arrival_time,0, pas));
      }
      queue<Passenger*> luggage_queue;
      queue<Passenger*> security_queue;
      double wait1=0;
      int missed_flights=0;
      while(!event.empty()){
          Event elimde = event.top();
          event.pop();
          time = elimde.time;
          if(elimde.type==2){
              wait1 += time - elimde.p->arrival_time;
              if(time > elimde.p->boarding_time){
                  missed_flights++;
              }
              available_security++;
              if(!security_queue.empty()){
                  Passenger* icinde = security_queue.front();
                  security_queue.pop();
                  event.push(Event(time + icinde->security_waiting,2,icinde));
                  available_security--;
              }
          }
          else if(elimde.type==1){
              available_luggage++;
              security_queue.push(elimde.p);
              if(!luggage_queue.empty()){
                  Passenger* licinde = luggage_queue.front();
                  luggage_queue.pop();
                  event.push(Event(time + licinde->luggage_waiting,1,licinde));
                  available_luggage--;
              }
              if(!security_queue.empty() && available_security>0){
                  Passenger* zicinde = security_queue.front();
                  security_queue.pop();
                  event.push(Event(time + zicinde->security_waiting,2,zicinde));
                  available_security--;
              }
          }
          else{
              luggage_queue.push(elimde.p);
              if(available_luggage>0){
                  Passenger* geldi = luggage_queue.front();
                  luggage_queue.pop();
                  event.push(Event(time + geldi->luggage_waiting, 1, geldi));
                  available_luggage--;
              }
          }
      }


    int time2 = 0;
    priority_queue<Event> event2;
    int available_security2 = n_of_security_counters;
    int available_luggage2 = n_of_luggage_counters;
    for (auto &pas : pass) {
        event2.push(Event(pas->arrival_time,0, pas));
    }
    priority_queue<Passenger*,vector<Passenger*>, firstfly> luggage_queue2;
    priority_queue<Passenger*,vector<Passenger*>, firstfly> security_queue2;
    double wait2=0;
    int missed_flights2=0;
    while(!event2.empty()){
        Event elimde = event2.top();
        event2.pop();
        time2 = elimde.time;
        if(elimde.type==2){
            wait2 += time2 - elimde.p->arrival_time;
            if(time2 > elimde.p->boarding_time){
                missed_flights2++;
            }
            available_security2++;
            if(!security_queue2.empty()){
                Passenger* icinde = security_queue2.top();
                security_queue2.pop();
                event2.push(Event(time2 + icinde->security_waiting,2,icinde));
                available_security2--;
            }
        }
        else if(elimde.type==1){
            available_luggage2++;
            security_queue2.push(elimde.p);
            if(!luggage_queue2.empty()){
                Passenger* licinde = luggage_queue2.top();
                luggage_queue2.pop();
                event2.push(Event(time2 + licinde->luggage_waiting,1,licinde));
                available_luggage2--;
            }
            if(!security_queue2.empty() && available_security2>0){
                Passenger* zicinde = security_queue2.top();
                security_queue2.pop();
                event2.push(Event(time2 + zicinde->security_waiting,2,zicinde));
                available_security2--;
            }
        }
        else{
            luggage_queue2.push(elimde.p);
            if(available_luggage2>0){
                Passenger* geldi = luggage_queue2.top();
                luggage_queue2.pop();
                event2.push(Event(time2 + geldi->luggage_waiting, 1, geldi));
                available_luggage2--;
            }
        }
    }

    int time3 = 0;
    priority_queue<Event> event3;
    int available_security3 = n_of_security_counters;
    int available_luggage3 = n_of_luggage_counters;
    for (auto &pas : pass) {
        event3.push(Event(pas->arrival_time,0, pas));
    }
    queue<Passenger*> luggage_queue3;
    queue<Passenger*> security_queue3;
    double wait3=0;
    int missed_flights3=0;
    while(!event3.empty()){
        Event elimde = event3.top();
        event3.pop();
        time3 = elimde.time;
        if(elimde.type==2){
            wait3 += time3 - elimde.p->arrival_time;
            if(time3 > elimde.p->boarding_time){
                missed_flights3++;
            }
            available_security3++;
            if(!security_queue3.empty()){
                Passenger* icinde = security_queue3.front();
                security_queue3.pop();
                event3.push(Event(time3 + icinde->security_waiting,2,icinde));
                available_security3--;
            }
        }
        else if(elimde.type==1){
            available_luggage3++;
            if(elimde.p->vip!="V"){
                security_queue3.push(elimde.p);
            }
            else{
                wait3 += time3 - elimde.p->arrival_time;
                if(time3 > elimde.p->boarding_time){
                    missed_flights3++;
                }
            }
            if(!luggage_queue3.empty()){
                Passenger* licinde = luggage_queue3.front();
                luggage_queue3.pop();
                event3.push(Event(time3 + licinde->luggage_waiting,1,licinde));
                available_luggage3--;
            }
            if(!security_queue3.empty() && available_security3>0){
                Passenger* zicinde = security_queue3.front();
                security_queue3.pop();
                event3.push(Event(time3 + zicinde->security_waiting,2,zicinde));
                available_security3--;
            }
        }
        else{
            luggage_queue3.push(elimde.p);
            if(available_luggage3>0){
                Passenger* geldi = luggage_queue3.front();
                luggage_queue3.pop();
                event3.push(Event(time3 + geldi->luggage_waiting, 1, geldi));
                available_luggage3--;
            }
        }
    }

    int time4 = 0;
    priority_queue<Event> event4;
    int available_security4 = n_of_security_counters;
    int available_luggage4 = n_of_luggage_counters;
    for (auto &pas : pass) {
        event4.push(Event(pas->arrival_time,0, pas));
    }
    priority_queue<Passenger*,vector<Passenger*>, firstfly> luggage_queue4;
    priority_queue<Passenger*,vector<Passenger*>, firstfly> security_queue4;
    double wait4=0;
    int missed_flights4=0;
    while(!event4.empty()){
        Event elimde = event4.top();
        event4.pop();
        time4 = elimde.time;
        if(elimde.type==2){
            wait4 += time4 - elimde.p->arrival_time;
            if(time4 > elimde.p->boarding_time){
                missed_flights4++;
            }
            available_security4++;
            if(!security_queue4.empty()){
                Passenger* icinde = security_queue4.top();
                security_queue4.pop();
                event4.push(Event(time4 + icinde->security_waiting,2,icinde));
                available_security4--;
            }
        }
        else if(elimde.type==1){
            available_luggage4++;
            if(elimde.p->vip!="V"){
                security_queue4.push(elimde.p);
            }
            else{
                wait4 += time4 - elimde.p->arrival_time;
                if(time4 > elimde.p->boarding_time){
                    missed_flights4++;
                }
            }
            if(!luggage_queue4.empty()){
                Passenger* licinde = luggage_queue4.top();
                luggage_queue4.pop();
                event4.push(Event(time4 + licinde->luggage_waiting,1,licinde));
                available_luggage4--;
            }
            if(!security_queue4.empty() && available_security4>0){
                Passenger* zicinde = security_queue4.top();
                security_queue4.pop();
                event4.push(Event(time4 + zicinde->security_waiting,2,zicinde));
                available_security4--;
            }
        }
        else{
            luggage_queue4.push(elimde.p);
            if(available_luggage4>0){
                Passenger* geldi = luggage_queue4.top();
                luggage_queue4.pop();
                event4.push(Event(time4 + geldi->luggage_waiting, 1, geldi));
                available_luggage4--;
            }
        }
    }

    int time5 = 0;
    priority_queue<Event> event5;
    int available_security5 = n_of_security_counters;
    int available_luggage5 = n_of_luggage_counters;
    for (auto &pas : pass) {
        event5.push(Event(pas->arrival_time,0, pas));
    }
    queue<Passenger*> luggage_queue5;
    queue<Passenger*> security_queue5;
    double wait5=0;
    int missed_flights5=0;
    while(!event5.empty()){
        Event elimde = event5.top();
        event5.pop();
        time5 = elimde.time;
        if(elimde.type==2){
            wait5 += time5 - elimde.p->arrival_time;
            if(time5 > elimde.p->boarding_time){
                missed_flights5++;
            }
            available_security5++;
            if(!security_queue5.empty()){
                Passenger* icinde = security_queue5.front();
                security_queue5.pop();
                event5.push(Event(time5 + icinde->security_waiting,2,icinde));
                available_security5--;
            }
        }
        else if(elimde.type==1){
            available_luggage5++;
            security_queue5.push(elimde.p);
            if(!luggage_queue5.empty()){
                Passenger* licinde = luggage_queue5.front();
                luggage_queue5.pop();
                event5.push(Event(time5 + licinde->luggage_waiting,1,licinde));
                available_luggage5--;
            }
            if(!security_queue5.empty() && available_security5>0){
                Passenger* zicinde = security_queue5.front();
                security_queue5.pop();
                event5.push(Event(time5 + zicinde->security_waiting,2,zicinde));
                available_security5--;
            }
        }
        else{
            if(elimde.p->luggage=="L") {
                luggage_queue5.push(elimde.p);
                if (available_luggage5 > 0) {
                    Passenger *geldi = luggage_queue5.front();
                    luggage_queue5.pop();
                    event5.push(Event(time5 + geldi->luggage_waiting, 1, geldi));
                    available_luggage5--;
                }
            }
            else{
                security_queue5.push(elimde.p);
                if(!security_queue5.empty() && available_security5>0){
                    Passenger* zicinde = security_queue5.front();
                    security_queue5.pop();
                    event5.push(Event(time5 + zicinde->security_waiting,2,zicinde));
                    available_security5--;
                }
            }
        }
    }

    int time6 = 0;
    priority_queue<Event> event6;
    int available_security6 = n_of_security_counters;
    int available_luggage6 = n_of_luggage_counters;
    for (auto &pas : pass) {
        event6.push(Event(pas->arrival_time,0, pas));
    }
    priority_queue<Passenger*,vector<Passenger*>, firstfly> luggage_queue6;
    priority_queue<Passenger*,vector<Passenger*>, firstfly> security_queue6;
    double wait6=0;
    int missed_flights6=0;
    while(!event6.empty()){
        Event elimde = event6.top();
        event6.pop();
        time6 = elimde.time;
        if(elimde.type==2){
            wait6 += time6 - elimde.p->arrival_time;
            if(time6 > elimde.p->boarding_time){
                missed_flights6++;
            }
            available_security6++;
            if(!security_queue6.empty()){
                Passenger* icinde = security_queue6.top();
                security_queue6.pop();
                event6.push(Event(time6 + icinde->security_waiting,2,icinde));
                available_security6--;
            }
        }
        else if(elimde.type==1){
            available_luggage6++;
            security_queue6.push(elimde.p);
            if(!luggage_queue6.empty()){
                Passenger* licinde = luggage_queue6.top();
                luggage_queue6.pop();
                event6.push(Event(time6 + licinde->luggage_waiting,1,licinde));
                available_luggage6--;
            }
            if(!security_queue6.empty() && available_security6>0){
                Passenger* zicinde = security_queue6.top();
                security_queue6.pop();
                event6.push(Event(time6 + zicinde->security_waiting,2,zicinde));
                available_security6--;
            }
        }
        else{
            if(elimde.p->luggage=="L") {
                luggage_queue6.push(elimde.p);
                if (available_luggage6 > 0) {
                    Passenger *geldi = luggage_queue6.top();
                    luggage_queue6.pop();
                    event6.push(Event(time6 + geldi->luggage_waiting, 1, geldi));
                    available_luggage6--;
                }
            }
            else{
                security_queue6.push(elimde.p);
                if(!security_queue6.empty() && available_security6>0){
                    Passenger* zicinde = security_queue6.top();
                    security_queue6.pop();
                    event6.push(Event(time6 + zicinde->security_waiting,2,zicinde));
                    available_security6--;
                }
            }
        }
    }


    int time7 = 0;
    priority_queue<Event> event7;
    int available_security7 = n_of_security_counters;
    int available_luggage7 = n_of_luggage_counters;
    for (auto &pas : pass) {
        event7.push(Event(pas->arrival_time,0, pas));
    }
    queue<Passenger*> luggage_queue7;
    queue<Passenger*> security_queue7;
    double wait7=0;
    int missed_flights7=0;
    while(!event7.empty()){
        Event elimde = event7.top();
        event7.pop();
        time7 = elimde.time;
        if(elimde.type==2){
            wait7 += time7 - elimde.p->arrival_time;
            if(time7 > elimde.p->boarding_time){
                missed_flights7++;
            }
            available_security7++;
            if(!security_queue7.empty()){
                Passenger* icinde = security_queue7.front();
                security_queue7.pop();
                event7.push(Event(time7 + icinde->security_waiting,2,icinde));
                available_security7--;
            }
        }
        else if(elimde.type==1){
            available_luggage7++;
            if(elimde.p->vip!="V"){
                security_queue7.push(elimde.p);
                if(!security_queue7.empty() && available_security7>0){
                   Passenger* zicinde = security_queue7.front();
                   security_queue7.pop();
                   event7.push(Event(time7 + zicinde->security_waiting,2,zicinde));
                   available_security7--;
                }
            }
            else{
                wait7 += time7 - elimde.p->arrival_time;
                if(time7>elimde.p->boarding_time){
                    missed_flights7++;
                }
            }
            if(!luggage_queue7.empty()){
                Passenger* licinde = luggage_queue7.front();
                luggage_queue7.pop();
                event7.push(Event(time7 + licinde->luggage_waiting,1,licinde));
                available_luggage7--;
            }
        }
        else{
            if(elimde.p->luggage=="L") {
                luggage_queue7.push(elimde.p);
                if (available_luggage7 > 0) {
                    Passenger *geldi = luggage_queue7.front();
                    luggage_queue7.pop();
                    event7.push(Event(time7 + geldi->luggage_waiting, 1, geldi));
                    available_luggage7--;
                }
            }
            else{
                if(elimde.p->vip!="V") {
                    security_queue7.push(elimde.p);
                    if (!security_queue7.empty() && available_security7 > 0) {
                        Passenger *zicinde = security_queue7.front();
                        security_queue7.pop();
                        event7.push(Event(time7 + zicinde->security_waiting, 2, zicinde));
                        available_security7--;
                    }
                }
                else{
                    wait7 += time7 - elimde.p->arrival_time;
                    if(time7 > elimde.p->boarding_time){
                        missed_flights7++;
                    }
                }
            }
        }
    }

    int time8 = 0;
    priority_queue<Event> event8;
    int available_security8 = n_of_security_counters;
    int available_luggage8 = n_of_luggage_counters;
    for (auto &pas : pass) {
        event8.push(Event(pas->arrival_time,0, pas));
    }
    priority_queue<Passenger*,vector<Passenger*>, firstfly> luggage_queue8;
    priority_queue<Passenger*,vector<Passenger*>, firstfly> security_queue8;
    double wait8=0;
    int missed_flights8=0;
    while(!event8.empty()){
        Event elimde = event8.top();
        event8.pop();
        time8 = elimde.time;
        if(elimde.type==2){
            wait8 += time8 - elimde.p->arrival_time;
            if(time8 > elimde.p->boarding_time){
                missed_flights8++;
            }
            available_security8++;
            if(!security_queue8.empty()){
                Passenger* icinde = security_queue8.top();
                security_queue8.pop();
                event8.push(Event(time8 + icinde->security_waiting,2,icinde));
                available_security8--;
            }
        }
        else if(elimde.type==1){
            available_luggage8++;
            if(elimde.p->vip!="V"){
                security_queue8.push(elimde.p);
                if(!security_queue8.empty() && available_security8>0){
                    Passenger* zicinde = security_queue8.top();
                    security_queue8.pop();
                    event8.push(Event(time8 + zicinde->security_waiting,2,zicinde));
                    available_security8--;
                }
            }
            else{
                wait8 += time8 - elimde.p->arrival_time;
                if(time8>elimde.p->boarding_time){
                    missed_flights8++;
                }
            }
            if(!luggage_queue8.empty()){
                Passenger* licinde = luggage_queue8.top();
                luggage_queue8.pop();
                event8.push(Event(time8 + licinde->luggage_waiting,1,licinde));
                available_luggage8--;
            }
        }
        else{
            if(elimde.p->luggage=="L") {
                luggage_queue8.push(elimde.p);
                if (available_luggage8 > 0) {
                    Passenger *geldi = luggage_queue8.top();
                    luggage_queue8.pop();
                    event8.push(Event(time8 + geldi->luggage_waiting, 1, geldi));
                    available_luggage8--;
                }
            }
            else{
                if(elimde.p->vip!="V") {
                    security_queue8.push(elimde.p);
                    if (!security_queue8.empty() && available_security8 > 0) {
                        Passenger *zicinde = security_queue8.top();
                        security_queue8.pop();
                        event8.push(Event(time8 + zicinde->security_waiting, 2, zicinde));
                        available_security8--;
                    }
                }
                else{
                    wait8 += time8 - elimde.p->arrival_time;
                    if(time8 > elimde.p->boarding_time){
                        missed_flights8++;
                    }
                }
            }
        }
    }


        // here, perform the output operation. in other words,
        // print your results into the file named <argv[2]>
        ofstream myfile;
        myfile.open(argv[2]);
        myfile << wait1 / n_of_passengers << " " << missed_flights << endl;
        myfile << wait2 / n_of_passengers << " " << missed_flights2 << endl;
        myfile << wait3 / n_of_passengers << " " << missed_flights3 << endl;
        myfile << wait4 / n_of_passengers << " " << missed_flights4 << endl;
        myfile << wait5 / n_of_passengers << " " << missed_flights5 << endl;
        myfile << wait6 / n_of_passengers << " " << missed_flights6 << endl;
        myfile << wait7 / n_of_passengers << " " << missed_flights7 << endl;
        myfile << wait8 / n_of_passengers << " " << missed_flights8 << endl;
        myfile.close();

        return 0;
    }