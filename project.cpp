#include <iostream>
#include <cstdint>
using namespace std;

bool senderNotReceiver;
uint32_t headerRows[5];
uint16_t summands[10];
uint16_t noCarrySum;
uint16_t finalChecksum;

bool senderNotReceiverFunction() {
    int choice;
    cout << "Enter 1 for Sender mode or 0 for Receiver mode: ";
    cin >> choice;
    return (choice == 1);
}

void readHeaderRows() {
    while (true) {
        cout << "Enter 5 unsigned 32-bit integers for the header rows:\n";
        for (int i = 0; i < 5; i++) {
            cin >> headerRows[i];
        }

        if (senderNotReceiver) {
            if ((headerRows[2] & 0xFFFF) != 0) {
                cout << "Error: Checksum bits must be 0." << endl;
                continue;
            }
        }
        break;
    }
}

void segmentation() {
    for (int i = 0; i < 5; i++) {
        summands[2 * i] = (headerRows[i] >> 16) & 0xFFFF;
        summands[2 * i + 1] = headerRows[i] & 0xFFFF;
    }
}

void sumWithCarryOver() {
    uint32_t sum = 0;

    for (int i = 0; i < 10; i++) {
        sum += summands[i];
        while (sum > 0xFFFF) {
            sum = (sum & 0xFFFF) + (sum >> 16);
        }
    }

    noCarrySum = (uint16_t)sum;
}

void bitwiseComplement() {
    finalChecksum = ~noCarrySum;
}

void printChecksum() {
    cout << "Checksum: " << finalChecksum << endl;
}

void anyError() {
    if (!senderNotReceiver) {
        if (finalChecksum == 0) {
            cout << "No error" << endl;
        } else {
            cout << "Error detected!" << endl;
        }
    }
}

int main() {
    senderNotReceiver = senderNotReceiverFunction();
    readHeaderRows();
    segmentation();
    sumWithCarryOver();
    bitwiseComplement();
    printChecksum();
    anyError();
    return 0;
}