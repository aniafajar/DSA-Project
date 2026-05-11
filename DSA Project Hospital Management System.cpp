#include<iostream>
#include<string>
using namespace std;
//=========TREATMENT HISTORY - Singly linked List=============
struct TreatmentNode {
    string treatment;
    TreatmentNode* next;
    TreatmentNode(string t){
	    treatment = t;
	    next = NULL;
	}
};

class TreatmentHistory {
private:
    TreatmentNode* head;

public:
    TreatmentHistory() {
        head = NULL;
    }

    ~TreatmentHistory() {
        TreatmentNode* temp = head;
        while (temp != NULL) {
            TreatmentNode* next = temp->next;
            delete temp;
            temp = next;
        }
    }

    void addTreatment(string t) {
        TreatmentNode* newNode = new TreatmentNode(t);
        if (head == NULL) {
            head = newNode;
        } 
		else {
            TreatmentNode* temp = head;
            while (temp->next != NULL)
                temp = temp->next;
            temp->next = newNode;
        }
    }

    void display() {
        if (head == NULL) {
            cout << "   No treatment records found." << endl;
            return;
        }
        TreatmentNode* temp = head;
        int count = 1;
        while (temp != NULL) {
            cout << "   " << count++ << ". " << temp->treatment << endl;
            temp = temp->next;
        }
    }

    bool hasRecords() {
        return head != NULL;
    }
};

struct Patient {
    int id;
    string name;
    int age;
    int priority;   
    string arrivalTime;
    TreatmentHistory history;

    Patient(){                                                               // default constructor 
	    id = 0; 
		name = "" ;
		age = 0; 
		priority = 5 ;
		arrivalTime = ""; 
    }
    Patient(int i, string n, int a, int p, string t){                      // parameterized constructor
    	id = i; 
		name = n; 
		age = a;
		priority = p; 
		arrivalTime = t;
	}
        

    string getPriorityLabel() {
        switch (priority) {
            case 1: return "CRITICAL";
            case 2: return "URGENT";
            case 3: return "NORMAL";
            case 4: return "LOW";
            case 5: return "NOT URGENT";
            default: return "UNKNOWN";
        }
    }
    
    void display() {
        cout << "   ID           : " << id << endl;
        cout << "   Name         : " << name << endl;
        cout << "   Age          : " << age << endl;
        cout << "   Priority     : " << priority << " (" << getPriorityLabel() << ")"<<endl;
        cout << "   Arrival Time : " << arrivalTime << endl;
    }
};

//==========Heap=============
class MinHeap {
private:
    Patient* heap;
    int capacity;
    int size;

    void siftUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[parent].priority > heap[index].priority) {
                swap(heap[parent], heap[index]);
                index = parent;
            } 
			else 
			    break;
        }
    }

    void siftDown(int index) {
        int smallest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left < size && heap[left].priority < heap[smallest].priority)
            smallest = left;
        if (right < size && heap[right].priority < heap[smallest].priority)
            smallest = right;

        if (smallest != index) {
            swap(heap[smallest], heap[index]);
            siftDown(smallest);
        }
    }

public:
    MinHeap(int cap = 100) : capacity(cap), size(0) {
        heap = new Patient[capacity];
    }

    bool isEmpty() { 
	    return size == 0; 
	}
    bool isFull()  { 
	    return size == capacity; 
	}

    void insert(Patient p) {
        if (isFull()) {
            cout << "   Emergency queue is full!\n";
            return;
        }
        heap[size] = p;
        siftUp(size);
        size++;
    }

    Patient extractMin() {
        Patient top = heap[0];
        heap[0] = heap[size - 1];
        size--;
        if (size > 0) siftDown(0);
        return top;
    }

    void display() {
        if (isEmpty()) {
            cout << "   Emergency queue is empty.\n";
            return;
        }
        
        Patient* temp = new Patient[size];
        for (int i = 0; i < size; i++){
        	temp[i] = heap[i];
		} 
        for (int i = 0; i < size - 1; i++){
        	for (int j = 0; j < size - i - 1; j++){
        	    if (temp[j].priority > temp[j + 1].priority)
                    swap(temp[j], temp[j + 1]);	
			}
                
		}
        cout << "   " << string(55, '-') << "\n";
        cout << "   " << left;
        cout.width(6);  cout << "ID";
        cout.width(18); cout << "Name";
        cout.width(8);  cout << "Age";
        cout.width(12); cout << "Priority";
        cout << "Arrival\n";
        cout << "   " << string(55, '-') << "\n";
        for (int i = 0; i < size; i++) {
            cout << "   ";
            cout.width(6);  cout << temp[i].id;
            cout.width(18); cout << temp[i].name;
            cout.width(8);  cout << temp[i].age;
            cout.width(12); cout << temp[i].getPriorityLabel();
            cout << temp[i].arrivalTime << "\n";
        }
        cout << "   " << string(55, '-') << "\n";
        delete[] temp;
    }

    ~MinHeap() { delete[] heap; }
};

//=============BST===========
struct BSTNode {
    Patient data;
    BSTNode* left;
    BSTNode* right;
    BSTNode(Patient p) : data(p), left(NULL), right(NULL) {}
};

class BST {
private:
    BSTNode* root;

    BSTNode* insert(BSTNode* node, Patient p) {
        if (node == NULL){
            return new BSTNode(p);	
		} 
        if (p.id < node->data.id){
        	node->left = insert(node->left, p);
		}    
        else if(p.id > node->data.id){
        	node->right = insert(node->right, p);
		}    
        else
            cout << "   Patient ID already exists!\n";
        return node;
    }

    BSTNode* findMin(BSTNode* node) {
        while (node->left != NULL)
            node = node->left;
        return node;
    }

    BSTNode* remove(BSTNode* node, int id, bool& found) {
        if (node == NULL){
            return NULL;	
		} 
        if (id < node->data.id)
            node->left = remove(node->left, id, found);
        else if (id > node->data.id)
            node->right = remove(node->right, id, found);
        else {
            found = true;
            if (node->left == NULL) {
                BSTNode* temp = node->right;
                delete node;
                return temp;
            } 
			else if (node->right == NULL) {
                BSTNode* temp = node->left;
                delete node;
                return temp;
            }
            BSTNode* successor = findMin(node->right);
            node->data = successor->data;
            bool dummy = false;
            node->right = remove(node->right, successor->data.id, dummy);
        }
        return node;
    }

    BSTNode* search(BSTNode* node, int id) {
        if (node == NULL || node->data.id == id) return node;
        if (id < node->data.id) return search(node->left, id);
        return search(node->right, id);
    }

    void inorder(BSTNode* node) {
        if (node == NULL) return;
        inorder(node->left);
        cout << "\n";
        node->data.display();
        inorder(node->right);
    }

    void destroyTree(BSTNode* node) {
        if (node == NULL) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

    bool isEmpty(BSTNode* node) { return node == NULL; }

public:
    BST() : root(NULL) {}

    void insert(Patient p) { root = insert(root, p); }

    void remove(int id) {
        bool found = false;
        root = remove(root, id, found);
        if (found)
            cout << "   Patient discharged successfully.\n";
        else
            cout << "   Patient ID not found.\n";
    }

    BSTNode* search(int id) { return search(root, id); }

    void displayAll() {
        if (root == NULL) {
            cout << "   No patient records found.\n";
            return;
        }
        inorder(root);
    }

    bool isEmpty() { return root == NULL; }

    ~BST() { destroyTree(root); }
};

//========QUEUE (General Waiting Room)=========
struct QueueNode {
    Patient data;
    QueueNode* next;
    QueueNode(Patient p) : data(p), next(NULL) {}
};

class Queue {
private:
    QueueNode* front;
    QueueNode* rear;
    int size;
public:
    Queue() : front(NULL), rear(NULL), size(0) {}

    bool isEmpty() { return front == NULL; }

    void enqueue(Patient p) {
        QueueNode* newNode = new QueueNode(p);
        if (rear == NULL) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        size++;
    }

    Patient dequeue() {
        Patient p = front->data;
        QueueNode* temp = front;
        front = front->next;
        if (front == NULL) rear = NULL;
        delete temp;
        size--;
        return p;
    }

    void display() {
        if (isEmpty()) {
            cout << "   General waiting queue is empty.\n";
            return;
        }
        QueueNode* temp = front;
        int count = 1;
        cout << "   " << string(55, '-') << "\n";
        cout << "   " << left;
        cout.width(6);  cout << "No.";
        cout.width(6);  cout << "ID";
        cout.width(18); cout << "Name";
        cout.width(8);  cout << "Age";
        cout << "Arrival\n";
        cout << "   " << string(55, '-') << "\n";
        while (temp != NULL) {
            cout << "   ";
            cout.width(6);  cout << count++;
            cout.width(6);  cout << temp->data.id;
            cout.width(18); cout << temp->data.name;
            cout.width(8);  cout << temp->data.age;
            cout << temp->data.arrivalTime << "\n";
            temp = temp->next;
        }
        cout << "   " << string(55, '-') << "\n";
    }

    ~Queue() {
        while (!isEmpty()) dequeue();
    }
};
void printHeader(string title) {
    cout << "   " << title << "\n";
}


int main() {
    MinHeap emergencyQueue(100);
    BST patientRecords;
    Queue generalQueue;
    int nextId = 1001;

    int choice;

    do {
        cout << "      .........HOSPITAL EMERGENCY MANAGEMENT SYSTEM..........\n";
        cout << "   1. Register New Patient\n";
        cout << "   2. Call Next Emergency Patient\n";
        cout << "   3. Call Next General Patient\n";
        cout << "   4. Search Patient by ID\n";
        cout << "   5. Display All Patient Records\n";
        cout << "   6. View Emergency Queue\n";
        cout << "   7. View General Waiting Queue\n";
        cout << "   8. Add Treatment Record\n";
        cout << "   9. View Treatment History\n";
        cout << "   10. Discharge Patient\n";
        cout << "   0. Exit\n";

        cout << "   Enter your choice: ";
        cin >> choice;
        cin.ignore();
        if (choice == 1) {
            printHeader("   -------REGISTER NEW PATIENT-------");

            string name;
			string arrivalTime;
            int age;
			int priority;

            cout << "   Auto-assigned Patient ID: " << nextId << "\n\n";
            cout << "   Enter Name        :"; getline(cin, name);
            cout << "   Enter Age         :"; cin >> age; cin.ignore();
            cout << "   Enter Arrival Time: "; getline(cin, arrivalTime);
            cout << "   Select Priority:"<<endl;
            cout << "   1 - Critical"<<endl;
            cout << "   2 - Urgent"<<endl;
            cout << "   3 - Normal"<<endl;
            cout << "   4 - Low"<<endl;
            cout << "   5 - Not Urgent"<<endl;
            cout << "   Enter Priority (1-5): "; cin >> priority; cin.ignore();

            if (priority < 1 || priority > 5) {
                cout << "   Invalid priority! Defaulting to 3 (Normal)."<<endl;
                priority = 3;
            }

            Patient p(nextId++, name, age, priority, arrivalTime);
            patientRecords.insert(p);

            if (priority <= 2)
                emergencyQueue.insert(p);
            else
                generalQueue.enqueue(p);
            
            cout << "\n -------Patient registered successfully!-------"<<endl;
            cout << "   Added to: " << (priority <= 2 ? "Emergency Queue" : "General Queue") <<endl;
        }

        else if (choice == 2) {
            printHeader("-------CALL NEXT EMERGENCY PATIENT-------");
            if (emergencyQueue.isEmpty()) {
                cout << " -------No emergency patients in queue-------"<<endl;
            } else {
                Patient p = emergencyQueue.extractMin();
                cout << " -------Calling next emergency patient:-------\n"<<endl;
                p.display();
            }
        }

       else if (choice == 3) {
            printHeader("-------CALL NEXT GENERAL PATIENT-------");
            if (generalQueue.isEmpty()) {
                cout << " -------  No patients in general waiting queue-------"<<endl;
            } else {
                Patient p = generalQueue.dequeue();
                cout << "  ------- Calling next general patient:-------\n"<<endl;
                p.display();
            }
        }

        else if (choice == 4) {
            printHeader("-------SEARCH PATIENT BY ID-------");
            int id;
            cout << "  Enter Patient ID:"; cin >> id; cin.ignore();
            BSTNode* result = patientRecords.search(id);
            if (result == NULL) {
                cout << "  Patient with ID " << id << " not found"<<endl;
            } else {
                cout << "\n   Patient found:\n"<<endl;
                result->data.display();
            }
        }

       else if (choice == 5) {
            printHeader("-------ALL PATIENT RECORDS (Sorted by ID)-------");
            patientRecords.displayAll();
        }

       else if (choice == 6) {
            printHeader("-------EMERGENCY QUEUE-------");
            emergencyQueue.display();
        }

        else if (choice == 7) {
            printHeader("-------GENERAL WAITING QUEUE-------");
            generalQueue.display();
        }

        else if (choice == 8) {
            printHeader("ADD TREATMENT RECORD");
            int id;
            string treatment;
            cout << "   Enter Patient ID       : "; cin >> id; cin.ignore();
            BSTNode* result = patientRecords.search(id);
            if (result == NULL) {
                cout << "   Patient not found."<<endl;
            } else {
                cout << "   Enter Treatment Details: "; getline(cin, treatment);
                result->data.history.addTreatment(treatment);
                cout << "   Treatment record added successfully."<<endl;
            }
        }

       else if (choice == 9) {
            printHeader("-------VIEW TREATMENT HISTORY-------");
            int id;
            cout << "   Enter Patient ID: "; cin >> id; cin.ignore();
            BSTNode* result = patientRecords.search(id);
            if (result == NULL) {
                cout << "   Patient not found."<<endl;
            } else {
                cout << "\n   Treatment history for " << result->data.name << ":\n"<<endl;
                result->data.history.display();
            }
        }

        else if (choice == 10) {
            printHeader("-------DISCHARGE PATIENT-------");
            int id;
            cout << "   Enter Patient ID to discharge: "; cin >> id; cin.ignore();
            patientRecords.remove(id);
        }

        else if (choice == 0) {
            cout << "\n ......  Thank you for using Hospital Emergency Management System......."<<endl;
            cout << " ......  Goodbye!......\n"<<endl;
        }

        else {
            cout << "   -------Invalid choice! Please enter a number from the menu.-------"<<endl;
        }

    } while (choice != 0);

    return 0;
}



    
