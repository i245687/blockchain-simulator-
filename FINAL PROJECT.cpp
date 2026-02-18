#include<iostream>
#include<string>
using namespace std;


string toHex(int x) {
    string hex = "0123456789abcdef";
    string out = "";
    out += hex[(x / 16) % 16];
    out += hex[x % 16];
    return out;
}

int hexToInt(char a, char b) {
    string hex = "0123456789abcdef";
    int high = hex.find(a);
    int low = hex.find(b);
    return high * 16 + low;
}

string encryptHash(string s) {
    string result = "";
    for (char c : s) {
        int value = int(c) + 5;
        result += toHex(value);
    }
    return result;
}


string decryptHash(string hexStr) {
    string result = "";
    for (int i = 0; i < hexStr.size(); i += 2) {
        int value = hexToInt(hexStr[i], hexStr[i + 1]);
        result += char(value - 5);
    }
    return result;
}
string hashPair(string left, string right) {
    return encryptHash(left + right);
}
class Transaction {
public:
    string txID;
    string sender;
    string receiver;
    double amount;

    Transaction() {}

    Transaction(string id, string s, string r, double a) {
        txID = id;
        sender = s;
        receiver = r;
        amount = a;
    }

    virtual void print() {
        cout << "Transaction ID: " << txID << endl;
        cout << "Sender: " << sender << endl;
        cout << "Receiver: " << receiver << endl;
        cout << "Amount: " << amount << endl;
    }
};


class PriorityTransaction : public Transaction {
public:
    int priorityLevel;

    PriorityTransaction(string id, string s, string r, double a, int p)
        : Transaction(id, s, r, a)
    {
        priorityLevel = p;
    }

    void print() override {
        cout << "[PRIORITY TRANSACTION]\n";
        cout << "Priority Level: " << priorityLevel << endl;
        Transaction::print();
    }
};


class Node {
public:
    Transaction* data;
    Node* next;

    Node(Transaction* t) {
        data = t;
        next = NULL;
    }
};

class TransactionQueue {
private:
    Node* front;
    Node* rear;

public:
    TransactionQueue() {
        front = rear = NULL;
    }

    bool isEmpty() {
        return front == NULL;
    }

    void enqueue(Transaction* t) {
        Node* newNode = new Node(t);

        if (isEmpty()) {
            front = rear = newNode;
        }
        else {
            rear->next = newNode;
            rear = newNode;
        }

        cout << "\nTransaction added to mempool.\n";
    }

    Transaction* dequeue() {
        if (isEmpty()) {
            cout << "\nMempool is empty!\n";
            return NULL;
        }

        Node* temp = front;
        front = front->next;

        Transaction* removed = temp->data;
        delete temp;

        return removed;
    }

    void showQueue() {
        if (isEmpty()) {
            cout << "\nMempool is empty!\n";
            return;
        }

        cout << "\n------ CURRENT MEMPOOL ------\n";
        Node* temp = front;

        while (temp != NULL) {
            temp->data->print();
            cout << "-----------------------------\n";
            temp = temp->next;
        }
    }
};
class Block {
public:
    int blockNumber;
    string timestamp;
    string transactions[10];
    int txCount;
    string previousHash;
    string currentHash;
    string merkleRoot;
    Block* next;


    Block(int num, string time, string txs[], int count, string prevHash = "", string curHash = "", string mRoot = "") {
        blockNumber = num;
        timestamp = time;
        txCount = count;

        for (int i = 0; i < count; i++)
            transactions[i] = txs[i];

        previousHash = prevHash;
        currentHash = curHash;
        merkleRoot = mRoot;
        next = NULL;
    }
};

class Blockchain {
public:
    Block* head;
    Block* tail;
    int blockCounter;

    Blockchain() {
        head = tail = NULL;
        blockCounter = 0;
    }

    void removeLastBlock() {
        if (head == NULL) return;
        if (head == tail) {
            delete head;
            head = tail = NULL;
            return;
        }

        Block* temp = head;
        while (temp->next != tail)
            temp = temp->next;

        delete tail;
        tail = temp;
        tail->next = NULL;
    }

    void printBlockchain() {
        Block* temp = head;
        while (temp != NULL) {
            cout << "\n--- Block #" << temp->blockNumber << " ---" << endl;
            cout << "Timestamp: " << temp->timestamp << endl;
            cout << "Previous Hash: " << temp->previousHash << endl;
            cout << "Current Hash: " << temp->currentHash << endl;
            cout << "Merkle Root: " << temp->merkleRoot << endl;

            cout << "Transactions:\n";
            for (int i = 0; i < temp->txCount; i++)
                cout << " - " << temp->transactions[i] << endl;

            temp = temp->next;
        }
    }
};
class MerkleNode {
public:
    string hash;
    MerkleNode* left;
    MerkleNode* right;

    MerkleNode(const string& h) {
        hash = h;
        left = NULL;
        right = NULL;
    }
};
class MerkleTree {
public:
    MerkleNode* root;

    MerkleTree() {
        root = NULL;
    }
    int i = 0;
    MerkleNode* Buildtree(string data[], int n) {
  
        i++;
        
        if (i >= n)
            return NULL;
        if (data[i] == "-1")
            return NULL;

        MerkleNode* root = new MerkleNode(data[i]);
        root->left = Buildtree(data, n);
        root->right = Buildtree(data, n);
        if (root->left == NULL && root->right == NULL) {
            root->hash = encryptHash(data[i]);
            return root;
        }
        else if (root->right == NULL && root->left!=NULL) {
            root->hash = hashPair(root->left->hash, root->left->hash);
            return root;
        }
        else {
            root->hash = hashPair(root->left->hash, root->right->hash);
            return root;
        }

    }
};
class RollbackNode {
public:
    int blockNumber;
    RollbackNode* next;

    RollbackNode(int bn) {
        blockNumber = bn;
        next = NULL;
    }
};

class RollbackStack {
public:
    RollbackNode* top;

    RollbackStack() {
        top = NULL;
    }
    void push(int bn) {
        RollbackNode* newNode = new RollbackNode(bn);
        newNode->next = top;
        top = newNode;
        cout << "Rollback record saved for Block #" << bn << endl;
    }

    int pop() {
        if (top == NULL) {
            cout << "Rollback stack is empty\n";
            return -1;
        }
        int bn = top->blockNumber;
        RollbackNode* temp = top;
        top = top->next;
        delete temp;
        cout << "Rollback record removed for Block #" << bn << endl;
        return bn;
    }

    bool isEmpty() {
        return (top == NULL);
    }

};
string transactionToString(Transaction* tx) {
    if (tx == NULL) return "";
    return "ID:" + tx->txID +
        " FROM:" + tx->sender +
        " TO:" + tx->receiver +
        " AMT:" + to_string(tx->amount);
}
const int MAX_TX_PER_BLOCK = 100;  

void mineBlock(TransactionQueue& mempool, Blockchain& chain, RollbackStack& rb) {
    
    if (mempool.isEmpty()) {
        cout << "\n[MINER] No transactions in mempool. Nothing to mine.\n";
        return;
    }

    
    string txStrings[MAX_TX_PER_BLOCK];
    for (int i = 0; i < MAX_TX_PER_BLOCK; i++) {
        txStrings[i] = "-1";
    }
    int txCount = 0;

    for (int k = 0; k < MAX_TX_PER_BLOCK && !mempool.isEmpty(); k++) {
        Transaction* t = mempool.dequeue();
        if (t != NULL) {
          
            txStrings[txCount] = transactionToString(t);
            txCount++;
         
            
        }
    }

    if (txCount == 0) {
        cout << "\n[MINER] No valid transactions dequeued.\n";
        return;
    }

    
    MerkleTree tree;
    tree.i = -1; 
    MerkleNode* root = tree.Buildtree(txStrings, txCount);

    if (root == NULL) {
        cout << "\n[MINER] Merkle tree build failed.\n";
        return;
    }

    string merkleRoot = root->hash;

    string previousHash;
    if (chain.head == NULL) {
        previousHash = "GENESIS";
    }
    else {
        previousHash = chain.tail->currentHash;
    }

    chain.blockCounter++;
    int blockNum = chain.blockCounter;

    
    string timestamp = "N/A";  

    
    string blockData = "BN:" + to_string(blockNum) +
        "|TS:" + timestamp +
        "|PH:" + previousHash +
        "|MR:" + merkleRoot;

    for (int i = 0; i < txCount; i++) {
        blockData += "|" + txStrings[i];
    }

    string currentHash = encryptHash(blockData);

    
    Block* newBlock = new Block(blockNum, timestamp, txStrings, txCount,
        previousHash, currentHash, merkleRoot);

    if (chain.head == NULL) {
        chain.head = chain.tail = newBlock;
    }
    else {
        chain.tail->next = newBlock;
        chain.tail = newBlock;
    }

    
    rb.push(blockNum);

    
    cout << "\n[MINER] Block #" << blockNum << " mined successfully.\n";
    cout << "        Previous Hash: " << previousHash << endl;
    cout << "        Merkle Root  : " << merkleRoot << endl;
    cout << "        Current Hash : " << currentHash << endl;
}

void rollbackLastBlock(Blockchain& chain, RollbackStack& rb) {
    if (rb.isEmpty()) {
        cout << "\n[ROLLBACK] No blocks to rollback.\n";
        return;
    }

    int bn = rb.pop();
    if (bn == -1) {
        cout << "\n[ROLLBACK] Stack error.\n";
        return;
    }

    
    if (chain.head == NULL) {
        cout << "\n[ROLLBACK] Blockchain is already empty.\n";
        return;
    }

    chain.removeLastBlock();
    chain.blockCounter--;  

    cout << "[ROLLBACK] Block #" << bn << " removed from blockchain.\n";
}
int main() {
    TransactionQueue mempool;
    Blockchain chain;
    RollbackStack rb;

    int choice;
    while (true) {
        cout << "\n1. Add Transaction\n";
        cout << "2. Mine Block\n";
        cout << "3. Print Blockchain\n";
        cout << "4. Print Mempool\n";
        cout << "5. Rollback Last Block\n";
        cout << "6. Exit\n";
        cin >> choice;

        if (choice == 1) {
            int pr;
            string s,id;
            string r;
            double amount;
            
            cout << endl<<"Enter tx ID : " << endl;
            cin >> id;
            cout << endl<<"Enter sender name : " << endl;
            cin >> s;
            cout << endl<<"Enter reciever name : " << endl;
            cin >> r;
            cout << endl<<"Enter the amount: " << endl;
            cin >> amount;
            cout << endl << "Enter the priority level: " << endl;
            cin >> pr;

            PriorityTransaction* p = new PriorityTransaction(id, s, r, amount, pr);
            mempool.enqueue(p);
            
        }
        else if (choice == 2) {
            mineBlock(mempool, chain, rb);
            
        }
        else if (choice == 3) {
            chain.printBlockchain();
        }
        else if (choice == 4) {
            mempool.showQueue();
        }
        else if (choice == 5) {
            rollbackLastBlock(chain, rb);
        }
        else break;
    }
}


