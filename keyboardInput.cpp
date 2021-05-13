#define    KEYBOARD_MONITOR_INPUT_FIFO_NAME        "KeyboardMonitorInputFifo"
int inputFilestream = -1;

#include <linux/input.h>
#include <cstdio>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <iosfwd>
#include <iostream>
#include <cstring>
#include <sstream>
#include <list>

#define BITS_PER_LONG (sizeof(long) * 8)
#define NBITS(x) ((((x)-1)/BITS_PER_LONG)+1)

using namespace std;

// from https://raspberry-projects.com/pi/programming-in-c/keyboard-programming-in-c/reading-raw-keyboard-input

void initializeKeyboardMonitor() {
    int result;

    printf("Making KeyboardMonitor queue...\n");
    result = mkfifo(KEYBOARD_MONITOR_INPUT_FIFO_NAME, 0777);
    if (result == 0) {
        printf("New queue created: %s\n", KEYBOARD_MONITOR_INPUT_FIFO_NAME);
    }

    printf("Process %d opening queue %s\n", getpid(), KEYBOARD_MONITOR_INPUT_FIFO_NAME);
    inputFilestream = open(KEYBOARD_MONITOR_INPUT_FIFO_NAME, (O_RDONLY | O_NONBLOCK));
    if (inputFilestream != -1) {
        printf("Opened input queue: %i\n", inputFilestream);
    }

    int pid2 = fork();
    if (pid2 == 0) {
        cout << "KeyboardMonitor child thread started" << endl;

        int fileDevice;
        int readDevice;
        int index;
        struct input_event InputEvent[64];
        int version;
        unsigned short id[4];
        unsigned long bit[EV_MAX][NBITS(KEY_MAX)];
        int noError = 1;
        stringstream ss1;

        if ((fileDevice = open("/dev/input/event0", O_RDONLY)) < 0) {
            perror("KeyboardMonitor can't open input device");
            noError = 0;
        }

        if (ioctl(fileDevice, EVIOCGVERSION, &version)) {
            perror("KeyboardMonitor can't get version");
            noError = 0;
        }

        if (noError) {
            ioctl(fileDevice, EVIOCGID, id);
            memset(bit, 0, sizeof(bit));
            ioctl(fileDevice, EVIOCGBIT(0, EV_MAX), bit[0]);
        }

        cout << "KeyboardMonitor child thread running" << endl;

        //Create an output filestream for this child thread
        int outputFilestream = -1;
        outputFilestream = open(KEYBOARD_MONITOR_INPUT_FIFO_NAME, (O_WRONLY | O_NONBLOCK));
        if (outputFilestream != -1)
            cout << "Opened output queue: " << outputFilestream << endl;

        while (noError) {
            readDevice = read(fileDevice, InputEvent, sizeof(struct input_event) * 64);

            if (readDevice < (int) sizeof(struct input_event)) {
                perror("KeyboardMonitor error reading - getKeyboardInput lost?");
                noError = 0;
            } else {
                for (index = 0; index < readDevice / sizeof(struct input_event); index++) {
                    //We have:
                    //	InputEvent[index].time		timeval: 16 bytes (8 bytes for seconds, 8 bytes for microseconds)
                    //	InputEvent[index].type		See input-event-codes.h
                    //	InputEvent[index].code		See input-event-codes.h
                    //	InputEvent[index].value		01 for keypress, 00 for release, 02 for autorepeat

                    if (InputEvent[index].type == EV_KEY) {
                        ss1.clear();
                        ss1.str(std::string());

                        if (InputEvent[index].value == 2) {
                            //This is an auto repeat of a held down key
                            //ss1 << (int)(InputEvent[index].code) << " Auto Repeat";
                        } else if (InputEvent[index].value == 1) {
                            //----- KEY DOWN -----
                            ss1 << "D" << (int) (InputEvent[index].code) << " ";
                        } else if (InputEvent[index].value == 0) {
                            //----- KEY UP -----
                            ss1 << "U" << (int) (InputEvent[index].code) << " ";
                        }

                        write(outputFilestream, (void *) ss1.str().c_str(), strlen(ss1.str().c_str()));
                    }
                }
            }
        }
        close(fileDevice);
        cout << "KeyboardMonitor child thread ending!" << endl;
        _exit(0);
    }
}

list<pair<char, int> > getKeyboardInput() {
    // Read up to 255 characters from the port if they are there
    // see input-event-codes.h
            list<pair<char, int> > inputs;
    if (inputFilestream != -1) {
        unsigned char InputBuffer[256];
        int BufferLength = read(inputFilestream, (void *) InputBuffer, 255);

        if (BufferLength > 0) {
            string s(reinterpret_cast<char*>(InputBuffer), BufferLength);
            istringstream iss(s);
            string word;
            while (iss >> word) {
                inputs.emplace_back(word.at(0), stoi(word.substr(1, word.length() - 1)));
            }
        }



    }
        return inputs;
}
