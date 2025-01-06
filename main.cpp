#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class Polynomial {
private:
    struct Node {
        int coeff;
        int exp;
        Node* next;
    };

    Node* head; 

public:
  
    Polynomial() : head(nullptr) {}

    Polynomial(const Polynomial& other) {
        if (other.head == nullptr) {
            head = nullptr;
            return;
        }

        head = new Node{ other.head->coeff, other.head->exp, nullptr };
        Node* current = head;
        Node* otherCurrent = other.head->next;

        while (otherCurrent != other.head) {
            current->next = new Node{ otherCurrent->coeff, otherCurrent->exp, nullptr };
            current = current->next;
            otherCurrent = otherCurrent->next;
        }

        current->next = head; // Complete the circular link
    }

    
    ~Polynomial() {
        clear();
    }

    Polynomial& operator=(const Polynomial& other) {
        if (this == &other) return *this;

        clear();

        if (other.head == nullptr) {
            head = nullptr;
            return *this;
        }

        head = new Node{ other.head->coeff, other.head->exp, nullptr };
        Node* current = head;
        Node* otherCurrent = other.head->next;

        while (otherCurrent != other.head) {
            current->next = new Node{ otherCurrent->coeff, otherCurrent->exp, nullptr };
            current = current->next;
            otherCurrent = otherCurrent->next;
        }

        current->next = head; 
        return *this;
    }

    friend istream& operator>>(istream& is, Polynomial& x) {
        int n;
        is >> n;
        x.clear();

        for (int i = 0; i < n; ++i) {
            int coeff, exp;
            is >> coeff >> exp;
            x.addTerm(coeff, exp);
        }

        return is;
    }

    friend ostream& operator<<(ostream& os, const Polynomial& x) {
        if (x.head == nullptr) {
            os << "0";
            return os;
        }

        Node* current = x.head;
        bool firstTerm = true;

        do {
            if (current->coeff != 0) {
                if (!firstTerm && current->coeff > 0) {
                    os << " + ";
                } else if (current->coeff < 0) {
                    os << " - ";
                }

                if (abs(current->coeff) != 1 || current->exp == 0) {
                    os << abs(current->coeff);
                }

                if (current->exp > 0) {
                    os << "x";
                    if (current->exp > 1) {
                        os << "^" << current->exp;
                    }
                }

                firstTerm = false;
            }

            current = current->next;
        } while (current != x.head);

        return os;
    }

    Polynomial operator+(const Polynomial& b) const {
        Polynomial result;
        Node* currentA = head;
        Node* currentB = b.head;

        do {
            result.addTerm(currentA->coeff, currentA->exp);
            currentA = currentA->next;
        } while (currentA != head);

        do {
            result.addTerm(currentB->coeff, currentB->exp);
            currentB = currentB->next;
        } while (currentB != b.head);

        return result;
    }

    Polynomial operator-(const Polynomial& b) const {
        Polynomial result;
        Node* currentA = head;
        Node* currentB = b.head;

        do {
            result.addTerm(currentA->coeff, currentA->exp);
            currentA = currentA->next;
        } while (currentA != head);

        do {
            result.addTerm(-currentB->coeff, currentB->exp);
            currentB = currentB->next;
        } while (currentB != b.head);

        return result;
    }
    Polynomial operator*(const Polynomial& b) const {
        Polynomial result;
        Node* currentA = head;

        do {
            Node* currentB = b.head;
            do {
                int newCoeff = currentA->coeff * currentB->coeff;
                int newExp = currentA->exp + currentB->exp;
                result.addTerm(newCoeff, newExp);
                currentB = currentB->next;
            } while (currentB != b.head);

            currentA = currentA->next;
        } while (currentA != head);

        return result;
    }

    int Evaluate(int x) const {
        if (head == nullptr) return 0;

        int result = 0;
        Node* current = head;

        do {
            result += current->coeff * pow(x, current->exp);
            current = current->next;
        } while (current != head);

        return result;
    }

private:
    void clear() {
        if (head == nullptr) return;

        Node* current = head->next;
        while (current != head) {
            Node* toDelete = current;
            current = current->next;
            delete toDelete;
        }

        delete head;
        head = nullptr;
    }

    void addTerm(int coeff, int exp) {
        if (coeff == 0) return;

        if (head == nullptr) {
            head = new Node{ coeff, exp, nullptr };
            head->next = head;
            return;
        }

        Node* current = head;
        Node* prev = nullptr;

        do {
            if (current->exp == exp) {
                current->coeff += coeff;
                if (current->coeff == 0) {
                    if (current == head && current->next == head) {
                        delete head;
                        head = nullptr;
                    } else {
                        prev->next = current->next;
                        if (current == head) head = current->next;
                        delete current;
                    }
                }
                return;
            }

            if (current->exp < exp) {
                Node* newNode = new Node{ coeff, exp, nullptr };
                if (prev == nullptr) {
                    newNode->next = head;

                    Node* temp = head;
                    while (temp->next != head) {
                        temp = temp->next;
                    }
                    temp->next = newNode;
                    head = newNode;
                } else {
                    newNode->next = current;
                    prev->next = newNode;
                }
                return;
            }

            prev = current;
            current = current->next;
        } while (current != head);

        Node* newNode = new Node{ coeff, exp, head };
        prev->next = newNode;
    }
};
int main() {
    Polynomial p1, p2;

    cout << "Enter the first polynomial:" << endl;
    cin >> p1;
    cout << "Enter the second polynomial:" << endl;
    cin >> p2;

    Polynomial sum = p1 + p2;
    Polynomial diff = p1 - p2;
    Polynomial prod = p1 * p2;

    cout << "First polynomial: " << p1 << endl;
    cout << "Second polynomial: " << p2 << endl;
    cout << "Sum: " << sum << endl;
    cout << "Difference: " << diff << endl;
    cout << "Product: " << prod << endl;

    int x;
    cout << "Enter a value to evaluate the first polynomial: ";
    cin >> x;
    cout << "Value of first polynomial at x = " << x << " is: " << p1.Evaluate(x) << endl;

    return 0;
}

