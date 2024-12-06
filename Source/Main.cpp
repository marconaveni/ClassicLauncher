#include "Application.h"

int main()
{
    ClassicLauncher::Application app;
    app.Init();
} 





// #include <iostream>
// #include <thread>
// #include <atomic>
// #include <chrono>

// class Worker {
// public:
//     Worker() : keepRunning(true), workerThread() {
//         std::cout << "Worker - starting thread\n";
//         workerThread = std::thread(&Worker::Work10Seconds, this);
//     }

//     ~Worker() {
//         std::cout << "Worker - stopping thread\n";
//         Stop();
//         if (workerThread.joinable()) {
//             workerThread.join();
//         }
//         std::cout << "Worker - thread stopped\n";
//     }

//     void Stop() {
//         keepRunning = false; // Sinaliza para encerrar
//     }

// private:
//     std::atomic<bool> keepRunning;
//     std::thread workerThread;

//     void Work10Seconds() {
//         std::cout << "Work10Seconds - entered\n";

//         for (uint8_t i = 0; i < 20; ++i) {
//             if (!keepRunning) {
//                 std::cout << "Work10Seconds - stopped early\n";
//                 break;
//             }
//             std::this_thread::sleep_for(std::chrono::milliseconds(500));
//             std::cout << "Work10Seconds - working\n";
//         }

//         std::cout << "Work10Seconds - exited\n";
//     }
// };

// int main() {
//     std::cout << "main - started\n";

//     {
//         Worker worker; // Cria o objeto Worker que inicia o thread
//         std::this_thread::sleep_for(std::chrono::seconds(20));

//         std::cout << "main - Signal worker to terminate\n";
//         worker.Stop(); // Sinaliza para o Worker encerrar
//     } // Destrói o objeto Worker, garantindo que o thread terminou

//     std::cout << "main - exited\n";
//     return 0;
// }

