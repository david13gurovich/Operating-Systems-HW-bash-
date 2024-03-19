// David Gurovich 318572047
// please compile with:  g++ ex3.cpp -o ex3.out -pthread -std=c++11
// the next line should be likethis: ./ex3.out config.txt
// I chose the 2nd option of the config file - only numbers 
// first line is not empty! the 3 numbers to each producer - first number is producer's id
// second number is the number of news and the third number is the size of the bounded queue
// between every producer's parameters there should be an empty line,
// finnaly after the last producer's parameters there should be an empty line, and then a line with
// a number that representing the size of the dispacher's bounded queue



#include <string.h>
#include <queue>
#include <vector>
#include <iostream>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <mutex>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <thread>
#include <fstream>
#include <string>

using namespace std;

vector<int> sizesArr;
vector<int> numOfStoriesForProd;
int counterProd = 0;
int counterCoEdit = 0;
int done = 0;
int done_counter =0;
vector<sem_t> semPopArr;
vector<sem_t> semPushArr;
sem_t semPopToScreen;
sem_t semPushToScreen;
pthread_mutex_t mutexToScreen;
int dispatcherSize;
queue<string> sportsQueue;
queue<string> weatherQueue;
queue<string> newsQueue;
vector<queue<string>> coEditorsQueueVec;
vector<pthread_mutex_t> mutexVectorProducers;
vector<pthread_mutex_t> mutexVectorCoEditors;

class BoundedQueue : public queue<string>
{
public:
    int _size;

    BoundedQueue(int size) // and semaphore
    {
        _size = size;
    }

    void pushBack(string const &story)
    {
        // semaphore
        queue<string>::push(story);
    }

    string popFirst()
    {
        if (!queue<string>::empty())
        {
            string popStory = queue<string>::front();
            queue<string>::pop();
            return popStory;
        }
    }
};

vector<BoundedQueue> boundedQueueArrProd; // global vector of bounded buffers between producers and the dispatcher
vector<BoundedQueue> boundedToScreen;

string makeNewStory(int id, string kindOfStory, int numOfKind)
{
    return "Producer" + to_string(id) + kindOfStory + to_string(numOfKind);
}

string getType(string fullStory)
{
    string copy;
    vector<string> v;
    copy = fullStory;
    string temp = "";
    for (int i = 0; i < fullStory.length(); ++i)
    {

        if (copy[i] == ' ')
        {
            v.push_back(temp);
            temp = "";
        }
        else
        {
            temp.push_back(copy[i]);
        }
    }
    v.push_back(temp);
    return v[2];
}

void *Dispatcher(void *arg)
{
    bool arrOfunDoneProd[sizesArr.size()];
    for (int j = 0; j < sizesArr.size(); j++)
    {
        arrOfunDoneProd[j] = false;
    }
    bool keepGoing = false;
    string story;
    string typeOfStory;
    for (int i = 0; i < sizesArr.size(); i++)
    {
        if (!arrOfunDoneProd[i])
            keepGoing = true;
    }
    while (keepGoing)
    {
        // sizesArr.size()
        for (int i = 0; i < sizesArr.size(); i++)
        {
            if (!arrOfunDoneProd[i])
            {
                sem_wait(&semPopArr[i]);
                pthread_mutex_lock(&mutexVectorProducers[i]);
                story = boundedQueueArrProd[i].popFirst();
                pthread_mutex_unlock(&mutexVectorProducers[i]);
                sem_post(&semPushArr[i]);
                if (story == "DONE")
                {
                    arrOfunDoneProd[i] = true;
                }
                else
                {
                    typeOfStory = getType(story);
                    if (typeOfStory == "NEWS") // 2
                    {
                        pthread_mutex_lock(&mutexVectorCoEditors[2]);
                        coEditorsQueueVec[2].push(story);
                        pthread_mutex_unlock(&mutexVectorCoEditors[2]);
                    }
                    if (typeOfStory == "WEATHER") // 1
                    {
                        pthread_mutex_lock(&mutexVectorCoEditors[1]);
                        coEditorsQueueVec[1].push(story);
                        pthread_mutex_unlock(&mutexVectorCoEditors[1]);
                    }
                    if (typeOfStory == "SPORTS") // 0
                    {
                        pthread_mutex_lock(&mutexVectorCoEditors[0]);
                        coEditorsQueueVec[0].push(story);
                        pthread_mutex_unlock(&mutexVectorCoEditors[0]);
                    }
                }
            }
        }
        keepGoing = false;
        for (int k = 0; k < sizesArr.size(); k++)
        {
            if (!arrOfunDoneProd[k])
                keepGoing = true;
        }
    }
    pthread_mutex_lock(&mutexVectorCoEditors[2]);
    coEditorsQueueVec[2].push("DONE");
    pthread_mutex_unlock(&mutexVectorCoEditors[2]);
    pthread_mutex_lock(&mutexVectorCoEditors[1]);
    coEditorsQueueVec[1].push("DONE");
    pthread_mutex_unlock(&mutexVectorCoEditors[1]);
    pthread_mutex_lock(&mutexVectorCoEditors[0]);
    coEditorsQueueVec[0].push("DONE");
    pthread_mutex_unlock(&mutexVectorCoEditors[0]);
    return NULL;
}

void *Producer(void *arg)
{
    int id = counterProd;
    counterProd++;
    bool keepGoing;
    string story;
    int sport = 0;   // 0
    int weather = 0; // 1
    int news = 0;    // 2
    if (numOfStoriesForProd[id])
        keepGoing = true;
    while (keepGoing)
    {

        if (numOfStoriesForProd[id]) // more stories to tell
        {
            int kindOfNews = numOfStoriesForProd[id] % 3;
            numOfStoriesForProd[id]--;
            if (kindOfNews == 0)
            {
                story = "Producer " + to_string(id + 1) + " SPORTS " + to_string(sport);
                sport++;
            }
            if (kindOfNews == 1)
            {
                story = "Producer " + to_string(id + 1) + " WEATHER " + to_string(weather);
                weather++;
            }
            if (kindOfNews == 2)
            {
                story = "Producer " + to_string(id + 1) + " NEWS " + to_string(news);
                news++;
            }

            int value;
            sem_getvalue(&semPushArr[id], &value);
            sem_wait(&semPushArr[id]);
            pthread_mutex_lock(&mutexVectorProducers[id]);
            boundedQueueArrProd[id].pushBack(story);
            sleep(0.5);
            pthread_mutex_unlock(&mutexVectorProducers[id]);
            sem_post(&semPopArr[id]);
            sem_getvalue(&semPopArr[id], &value);
        }
        else
        {
            keepGoing = false;
            // semaphore update and mutex lock
            sem_wait(&semPushArr[id]);
            pthread_mutex_lock(&mutexVectorProducers[id]);
            boundedQueueArrProd[id].pushBack("DONE");
            pthread_mutex_unlock(&mutexVectorProducers[id]);
            sem_post(&semPopArr[id]);
            // semaphore update and mutex lock
        }
    }
}

void *CoEditor(void *arg)
{
    int id = counterCoEdit;
    counterCoEdit++;
    //sleep(1);
    string topStory = "";
    bool isEmpty;
    while (topStory != "DONE")
    {
        pthread_mutex_lock(&mutexVectorCoEditors[id]);
        isEmpty = coEditorsQueueVec[id].empty();
        pthread_mutex_unlock(&mutexVectorCoEditors[id]);
        while (isEmpty) // wiating to dispatcher
        {
            sleep(0.1);
            pthread_mutex_lock(&mutexVectorCoEditors[id]);
            isEmpty = coEditorsQueueVec[id].empty();
            pthread_mutex_unlock(&mutexVectorCoEditors[id]);
        }
        pthread_mutex_lock(&mutexVectorCoEditors[id]);
        topStory = coEditorsQueueVec[id].front(); // gettings first story
        coEditorsQueueVec[id].pop();              // getting the story out of queue
        pthread_mutex_unlock(&mutexVectorCoEditors[id]);
        sleep(0.1);                 // edits
        sem_wait(&semPushToScreen); // from here
        pthread_mutex_lock(&mutexToScreen);
        boundedToScreen[0].pushBack(topStory); // pushing the story to final queue
        pthread_mutex_unlock(&mutexToScreen);
        sem_post(&semPopToScreen); // to here
        if(topStory == "DONE")
        {
            done_counter ++ ;
        }
    }
    // sem_wait(&semPushToScreen);
    // pthread_mutex_lock(&mutexToScreen);
    // boundedToScreen[0].pushBack(topStory); // pushing "DONE"
    // pthread_mutex_unlock(&mutexToScreen);
    // sem_post(&semPopToScreen);
}

void readFromFile()
{
    fstream newfile;
    newfile.open("config.txt"); // open a file to perform read operation using file object
    if (newfile.is_open())
    { // checking whether the file is open
        string line;
        while (getline(newfile, line))
        { // read data from file object and put it into string.
            int producerNum = stoi(line);
            getline(newfile, line);
            if (newfile.eof())
            {
                dispatcherSize = producerNum;
                newfile.close();
                return;
            }
            int numberOfStories = stoi(line);
            numOfStoriesForProd.push_back(numberOfStories);
            getline(newfile, line);
            int queueSize = stoi(line);
            sizesArr.push_back(queueSize);
            getline(newfile, line); // empty line
        }
    }
}

void* screenManager(void * args)
{
    string storyToScreen = "";
    while (!boundedToScreen[0].empty() || done != 3)
    {
        sem_wait(&semPopToScreen);
        pthread_mutex_lock(&mutexToScreen);
        storyToScreen = boundedToScreen[0].popFirst();
        pthread_mutex_unlock(&mutexToScreen);
        sem_post(&semPushToScreen);
        if(storyToScreen == "DONE"){
            done +=1;
        }
        else if(storyToScreen != "")
            cout << storyToScreen << endl;
        storyToScreen = "";
    }
    cout << "DONE" << endl;
}

void inializeSemaphores()
{
    for (int i = 0; i < sizesArr.size(); i++)
    {
        sem_t semPop;
        sem_t semPush;
        pthread_mutex_t mutex;
        pthread_mutex_init(&mutex, NULL);
        mutexVectorProducers.push_back(mutex);
        sem_init(&semPop, 0, 0);
        sem_init(&semPush, 0, sizesArr[i]);
        semPopArr.push_back(semPop);
        semPushArr.push_back(semPush);
    }
    for (int k = 0; k < 3; k++)
    {
        pthread_mutex_t mutex;
        pthread_mutex_init(&mutex, NULL);
        mutexVectorCoEditors.push_back(mutex);
    }
    coEditorsQueueVec.push_back(newsQueue);
    coEditorsQueueVec.push_back(weatherQueue);
    coEditorsQueueVec.push_back(sportsQueue);
    pthread_mutex_init(&mutexToScreen, NULL);
    sem_init(&semPopToScreen, 0, 0);
    sem_init(&semPushToScreen, 0, dispatcherSize);
}

void destroyAll()
{
    for (int i = 0; i < sizesArr.size(); i++)
    {
        pthread_mutex_destroy(&mutexVectorProducers[i]);
        if(i < 3)
            pthread_mutex_destroy(&mutexVectorCoEditors[i]);
        pthread_mutex_destroy(&mutexToScreen);
        sem_destroy(&semPopArr[i]);
        sem_destroy(&semPushArr[i]);
    }
    sem_destroy(&semPopToScreen);
    sem_destroy(&semPushToScreen);
}

int main()
{

    /*
    get values from a config file
    get number of prodecers
    */
    readFromFile();
    inializeSemaphores();
    pthread_t th[sizesArr.size()];
    BoundedQueue boundedToScreenQ = BoundedQueue(dispatcherSize);
    boundedToScreen.push_back(boundedToScreenQ);
    // sizesArr.size()
    for (int i = 0; i < sizesArr.size(); i++)
    {

        BoundedQueue boundedQueue = BoundedQueue(sizesArr[i]);

        boundedQueueArrProd.push_back(boundedQueue);
        // set val semun
        if (pthread_create(&th[i], NULL, &Producer, NULL) != 0)
        {
            perror("Failed to create thread");
        }
        sleep(0.1);
    }
    pthread_t disptcher;
    if (pthread_create(&disptcher, NULL, &Dispatcher, NULL) != 0)
    {
        perror("Failed to create thread");
    }
    pthread_t coEditors[3];
    for (int p = 0; p < 3; p++)
    {
        sleep(0.1);
        if (pthread_create(&coEditors[p], NULL, &CoEditor, NULL) != 0)
        {
            perror("Failed to create thread");
        }
    }
    pthread_t screenM;
    if (pthread_create(&disptcher, NULL, &screenManager, NULL) != 0)
    {
        perror("Failed to create thread");
    }


    
    for (int y = 0; y < sizesArr.size(); y++)
    {
        if (pthread_join(th[y], NULL) != 0)
        {
            perror("Failed to join thread");
        }
    }
    if (pthread_join(disptcher, NULL) != 0)
    {
        perror("Failed to join thread");
    }
    for (int g = 0; g < 3; g++)
    {
        if (pthread_join(coEditors[g], NULL) != 0)
        {
            perror("Failed to join thread");
        }
    }
    destroyAll();
    return 0;
}
