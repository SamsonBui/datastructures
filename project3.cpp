#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>

template <typename T>
class NovelQueue {
private:
    std::vector<T> queue;

public:
    // Enqueue a job (add job to the back of the queue)
    void enqueue(T job) {
        queue.push_back(job);
        std::cout << "Job ID: " << job.getJobID() << ", Priority: " << job.getPriority()
                  << ", Job Type: " << job.getJobType() << ", CPU Time Consumed: " 
                  << job.getCPUTimeConsumed() << ", Memory Consumed: " 
                  << job.getMemoryConsumed() << std::endl;
    }

    // Dequeue a job (remove job from the front of the queue)
    T dequeue() {
        if (queue.empty()) {
            throw std::runtime_error("Queue is empty");
        }
        T job = queue.front();
        queue.erase(queue.begin());
        std::cout << "Dequeued Job: " << std::endl;
        job.display();
        return job;
    }

    // Modify a job's attribute by ID
    void modifyJob(int jobID, int attribute, int newValue) {
    auto it = std::find_if(queue.begin(), queue.end(), [&](T& job) {
        return job.getJobID() == jobID;
    });

    if (it != queue.end()) {
        std::cout << "Changed Job ID " << jobID << " field " << attribute << " to " << newValue << ":" << std::endl;
        it->modifyAttribute(attribute, newValue);
        it->display();
    } else {
        std::cout << "Job with ID " << jobID << " not found in the queue (perhaps dequeued)." << std::endl;
    }
    }

    // Promote a job by moving it up in the queue by a certain number of positions
    void promoteJob(int jobID, int positions) {
    auto it = std::find_if(queue.begin(), queue.end(), [&](T& job) {
        return job.getJobID() == jobID;
    });

    if (it != queue.end()) {
        int index = std::distance(queue.begin(), it);
        if (index - positions >= 0) {
            std::swap(queue[index], queue[index - positions]);
            std::cout << "Promoted Job ID " << jobID << " by " << positions << " Position(s):" << std::endl;
            queue[index - positions].display();
        }
    } else {
        std::cout << "Job with ID " << jobID << " not found in the queue (perhaps dequeued)." << std::endl;
    }
    }

    // Display all jobs in the queue
    void displayQueue() const {
        for (const auto& job : queue) {
            job.display();
        }
    }

    // Sort jobs by Job ID
    void sortByJobID() {
        std::sort(queue.begin(), queue.end(), [](const T& a, const T& b) {
            return a.getJobID() < b.getJobID();
        });
    }

    // Sort jobs by Priority
    void sortByPriority() {
        std::sort(queue.begin(), queue.end(), [](const T& a, const T& b) {
            return a.getPriority() < b.getPriority();
        });
    }

    // Print the total number of jobs in the queue
    void printQueueSize() const {
        std::cout << "Number of elements in the queue: " << queue.size() << std::endl;
    }

    // Reorder queue by priority and display
    void reorderQueue() {
        sortByPriority();
        std::cout << "Reordered Queue:" << std::endl;
        displayQueue();
    }
};

class Job {
private:
    int jobID;
    int priority;
    int jobType;
    int cpuTimeConsumed;
    int memoryConsumed;

public:
    // Constructor
    Job(int id, int prio, int type, int cpuTime, int memory)
        : jobID(id), priority(prio), jobType(type), cpuTimeConsumed(cpuTime), memoryConsumed(memory) {}

    // Getters
    int getJobID() const { return jobID; }
    int getPriority() const { return priority; }
    int getJobType() const { return jobType; }
    int getCPUTimeConsumed() const { return cpuTimeConsumed; }
    int getMemoryConsumed() const { return memoryConsumed; }

    // Modify job attributes
    void modifyAttribute(int attribute, int newValue) {
        switch (attribute) {
            case 1: priority = newValue; break;
            case 2: jobType = newValue; break;
            case 3: cpuTimeConsumed = newValue; break;
            case 4: memoryConsumed = newValue; break;
            default: std::cout << "Invalid attribute" << std::endl;
        }
    }

    // Display job details
    void display() const {
        std::cout << "Job ID: " << jobID << ", Priority: " << priority
                  << ", Job Type: " << jobType << ", CPU Time Consumed: " << cpuTimeConsumed
                  << ", Memory Consumed: " << memoryConsumed << std::endl;
    }
};

int main() {
    NovelQueue<Job> jobQueue;

    // Open the input file
    std::ifstream inputFile("input1-2.txt");
    std::string line;
    int totalJobs = 0;

    if (inputFile.is_open()) {
        // Read the number of operations first
        std::getline(inputFile, line);
        std::istringstream iss(line);
        iss >> totalJobs;

        std::cout << "Start Enqueueing Jobs: " << std::endl;

        // Process each command
        while (std::getline(inputFile, line)) {
            char command;
            std::istringstream cmdStream(line);
            cmdStream >> command;

            if (command == 'A') {
                // Add job (A JobID Priority JobType CPUTime Memory)
                int jobID, priority, jobType, cpuTime, memory;
                cmdStream >> jobID >> priority >> jobType >> cpuTime >> memory;
                Job newJob(jobID, priority, jobType, cpuTime, memory);
                jobQueue.enqueue(newJob);
            } else if (command == 'P') {
                // Promote job (P JobID Positions)
                int jobID, positions;
                cmdStream >> jobID >> positions;
                jobQueue.promoteJob(jobID, positions);
            } else if (command == 'C') {
                // Modify job attribute (C JobID Attribute NewValue)
                int jobID, attribute, newValue;
                cmdStream >> jobID >> attribute >> newValue;
                jobQueue.modifyJob(jobID, attribute, newValue);
            } else if (command == 'R') {
                // Dequeue (R)
                jobQueue.dequeue();
            } else if (command == 'D') {
                // Display the queue (D)
                jobQueue.displayQueue();
            } else if (command == 'O') {
                // Sort by Job ID (O 1) or Priority (O 2)
                int sortOption;
                cmdStream >> sortOption;
                if (sortOption == 1) {
                    jobQueue.sortByJobID();
                } else if (sortOption == 2) {
                    jobQueue.sortByPriority();
                }
                std::cout << "List of jobs in sorted order:" << std::endl;
                jobQueue.displayQueue();
            } else if (command == 'L') {
                // Print queue size (L)
                jobQueue.printQueueSize();
            } else if (command == 'N') {
                // Reorder and display queue
                jobQueue.reorderQueue();
            }
        }
    } else {
        std::cerr << "Unable to open input file." << std::endl;
    }

    return 0;
}