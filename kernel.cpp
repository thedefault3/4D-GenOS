
---

# 2) kernel4d_sim.cpp (full source)

Save this as `kernel4d_sim.cpp`. It's intentionally self-contained and documented.

```cpp
// kernel4d_sim.cpp
// Harmless theatrical "4D Kernel" setup & boot simulator.
// Build: g++ -std=c++11 -O2 kernel4d_sim.cpp -o kernel4d_sim
// Run: ./kernel4d_sim
//
// IMPORTANT: This program performs only local filesystem operations in a directory
// named ./4d_kernel_env. It does NOT touch /boot, does NOT load kernel modules,
// does NOT require root. It's purely a simulation for demonstration/prank purposes.

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <iomanip>
#include <random>
#include <ctime>
#include <sys/stat.h>

using namespace std;
using namespace std::chrono;

static const string ESC = "\x1b[";
static const string RESET = ESC + "0m";
static const string BOLD = ESC + "1m";
static const string RED = ESC + "31m";
static const string GREEN = ESC + "32m";
static const string YELLOW = ESC + "33m";
static const string BLUE = ESC + "34m";
static const string MAG = ESC + "35m";
static const string CYAN = ESC + "36m";

// Utility: sleep helper
static void sleep_ms(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

// Animated typing
static void slow_print(const string &s, int ch_ms = 4) {
    for (char c : s) {
        cout << c << flush;
        sleep_ms(ch_ms);
    }
    cout << endl;
}

// Simple progress bar (inline)
static void progress_bar(float seconds, const string &label = "") {
    const int width = 36;
    int steps = max(8, (int)(width));
    for (int i = 0; i <= steps; ++i) {
        int pct = (int)((i / (float)steps) * 100.0f);
        int filled = i;
        int empty = steps - filled;
        cout << "\r" << CYAN;
        if (!label.empty()) {
            cout << label << " ";
        }
        cout << "[" << string(filled, '#') << string(empty, '-') << "] "
             << setw(3) << pct << "%" << RESET << flush;
        sleep_ms((int)(seconds * 1000.0f / steps));
    }
    cout << endl;
}

// Random hex artifact
static string random_hex(size_t n) {
    static std::mt19937 rng((unsigned)time(nullptr));
    static const char *hex = "0123456789abcdef";
    string s;
    s.reserve(n);
    for (size_t i=0;i<n;i++) s.push_back(hex[rng() % 16]);
    return s;
}

// Ensure directory exists (simple)
static bool ensure_dir(const string &d) {
#if defined(_WIN32)
    int r = _mkdir(d.c_str());
#else
    mode_t mode = 0755;
    int r = mkdir(d.c_str(), mode);
#endif
    // return true if exists or created ok
    struct stat st;
    if (stat(d.c_str(), &st) == 0 && S_ISDIR(st.st_mode)) return true;
    return (r == 0);
}

// Write small placeholder file
static void write_file(const string &path, const string &content, bool binary=false) {
    ios::openmode m = ios::out;
    if (binary) m = ios::out | ios::binary;
    ofstream ofs(path.c_str(), m);
    ofs << content;
    ofs.close();
}

// Create a fake binary placeholder (random bytes in hex displayed but saved as text)
static void create_fake_image(const string &path, size_t kb) {
    ofstream ofs(path.c_str(), ios::out | ios::binary);
    std::mt19937 rng((unsigned)time(nullptr));
    std::uniform_int_distribution<int> dist(0, 255);
    size_t total = kb * 1024;
    vector<unsigned char> buf;
    buf.reserve(total);
    for (size_t i=0;i<total;i++) buf.push_back((unsigned char)dist(rng));
    ofs.write((char*)buf.data(), buf.size());
    ofs.close();
}

// Small helper to print a decorative header
static void print_header() {
    cout << MAG << BOLD;
    cout << "  ____  _  _  ____    ____    _  _  _  _  _  _ " << endl;
    cout << " / ___|| || ||  _ \\  / ___|  / \\/ \\/ \\/ \\/ \\/ \\" << endl;
    cout << "| |  _ | || || |_) | \\___ \\ / /\\ /\\ /\\ /\\ /\\ /" << endl;
    cout << "| |_| || || ||  _ <   ___) / /__\\/__\\/__\\/__\\/ " << endl;
    cout << " \\____||_||_||_| \\_\\ |____/\\____/\\____/\\____/  " << endl;
    cout << RESET << endl;
}

// Main simulated setup sequence
static void run_simulation() {
    print_header();
    slow_print(CYAN + "shadow 4D kernel installer - v4.0-sim" + RESET, 2);
    slow_print(YELLOW + "Preparing local sandbox environment..." + RESET, 2);
    sleep_ms(200);

    string envdir = "4d_kernel_env";
    if (!ensure_dir(envdir)) {
        slow_print(RED + "Failed to create environment directory: " + envdir + RESET);
        return;
    }
    slow_print(GREEN + "Environment directory: ./" + envdir + RESET);
    sleep_ms(200);

    // Step 1: generate fake kernel image
    slow_print(YELLOW + "Generating 4D kernel image..." + RESET);
    progress_bar(1.3f, "module-compile");
    string imgPath = envdir + "/4d-kernel.img";
    create_fake_image(imgPath, 48); // 48 KB placeholder
    slow_print(GREEN + "-> created " + imgPath + RESET);
    sleep_ms(250);

    // Step 2: generate fake initramfs
    slow_print(YELLOW + "Creating compressed initramfs (simulated)..." + RESET);
    progress_bar(0.9f, "initramfs-pack");
    string initPath = envdir + "/4d-initramfs.cpio.gz";
    write_file(initPath, "SIMULATED_INITRAMFS_ARCHIVE_CONTENT\n", false);
    slow_print(GREEN + "-> created " + initPath + RESET);
    sleep_ms(250);

    // Step 3: write config
    slow_print(YELLOW + "Writing runtime configuration..." + RESET);
    string conf = R"conf(# 4D Kernel simulated config
[core]
name = "4d-kernel-sim"
version = "4.0-sim"
mode = "temporal-safe"
max_dimensions = 4

[modules]
module0 = "chrono_scheduler"
module1 = "entropy-bridge"
module2 = "slice-manager"
module3 = "quantum-sandbox"
)conf";
    string confPath = envdir + "/4d.conf";
    write_file(confPath, conf, false);
    slow_print(GREEN + "-> wrote " + confPath + RESET);
    sleep_ms(200);

    // Step 4: generate artifact hex
    slow_print(YELLOW + "Generating integrity artifact (sha-sim)..." + RESET);
    progress_bar(0.7f, "artifact");
    string art = random_hex(128);
    string artPath = envdir + "/artifact.hex";
    write_file(artPath, art + "\n", false);
    slow_print(GREEN + "-> artifact saved to " + artPath + RESET);
    sleep_ms(200);

    // Step 5: create fake systemd unit sample (local)
    slow_print(YELLOW + "Preparing service descriptor (sample)..." + RESET);
    string svc = R"(# 4d-kernel.service.sample (DO NOT ENABLE - sample only)
[Unit]
Description=4D Kernel Simulation (sample)
After=network.target

[Service]
Type=oneshot
ExecStart=/bin/echo \"This is a sample service file. DO NOT enable on production.\"
RemainAfterExit=yes

[Install]
WantedBy=multi-user.target
)";
    string svcPath = envdir + "/4d-kernel.service.sample";
    write_file(svcPath, svc, false);
    slow_print(GREEN + "-> wrote " + svcPath + RESET);
    sleep_ms(200);

    // Step 6: fake module listing & verification
    slow_print(CYAN + "Verifying image and modules..." + RESET);
    vector<string> modules = {
        "chrono_scheduler.kmod",
        "entropy_bridge.kmod",
        "slice_manager.kmod",
        "quantum_sandbox.kmod",
        "retro_compat.kmod"
    };
    for (size_t i=0;i<modules.size();++i) {
        string m = modules[i];
        cout << setw(2) << (i+1) << ". " << MAG << m << RESET << " ";
        progress_bar(0.35f, m);
        cout << "  " << GREEN << "OK" << RESET << endl;
    }
    sleep_ms(120);

    // Integrity "check"
    slow_print(YELLOW + "Performing integrity check (simulated SHA256)..." + RESET);
    progress_bar(1.0f, "sha256-sim");
    string checksum = random_hex(64);
    cout << GREEN << "sha256: " << checksum << RESET << endl;
    sleep_ms(180);

    slow_print(GREEN + BOLD + "4D Kernel image prepared successfully (SIMULATION MODE)" + RESET);
    sleep_ms(250);

    // Offer simulated boot
    slow_print(CYAN + "Would you like to simulate boot now? (y/N)" + RESET, 2);
    cout << "> " << flush;
    string resp;
    getline(cin, resp);
    if (!resp.empty() && (resp[0]=='y' || resp[0]=='Y')) {
        // Simulated boot sequence
        slow_print(MAG + "Starting simulated boot..." + RESET, 6);
        simulate_boot(envdir);
    } else {
        slow_print(YELLOW + "Skipping boot simulation. Inspect files in ./" + envdir + RESET);
    }
    slow_print(CYAN + "Simulation complete. Note: this was a local-only theatrical simulation." + RESET);
}

// Simulate staged boot logs
static void simulate_boot(const string &envdir) {
    vector<string> early = {
        "Booting 4D Kernel Simulator v4.0-sim",
        "Setting up CPU micro-slices [OK]",
        "Initializing chrono-scheduler [OK]",
        "Probing pseudo-hardware: temporal bus, entropy bridge [OK]",
        "Mounting pseudo rootfs: /simroot [RO]",
        "Loading main modules: chrono_scheduler, slice_manager, quantum_sandbox"
    };
    vector<string> mid = {
        "Activating inter-slice comms [OK]",
        "Registering 4th-dimension manager [OK]",
        "Spawning temporal worker threads x16",
        "Entropy bridge calibration: 0.9 -> 0.98",
        "Virtual devices: /dev/slice0, /dev/slice1 [OK]"
    };
    vector<string> late = {
        "Starting user-land shim (simulated)",
        "Applying policy: temporal-safe-mode",
        "Network stack: disabled (simulation)",
        "Loading artifact: " + envdir + "/artifact.hex",
        "Kernel prompt: 4d#"
    };

    for (auto &l : early) {
        cout << BLUE << "[" << time_now() << "] " << RESET << l << endl;
        sleep_ms(450);
    }
    for (auto &l : mid) {
        cout << CYAN << "[" << time_now() << "] " << RESET << l << endl;
        sleep_ms(400);
    }
    // Insert a "weird" event then recover
    cout << RED << "[" << time_now() << "] " << RESET << "WARNING: Temporal skew detected on slice1" << endl;
    sleep_ms(800);
    cout << YELLOW << "[" << time_now() << "] " << RESET << "Attempting corrective drift compensation..." << endl;
    progress_bar(1.2f, "drift-correct");
    cout << GREEN << "[" << time_now() << "] " << RESET << "Compensation complete. No data loss." << endl;
    sleep_ms(350);

    for (auto &l : late) {
        cout << GREEN << "[" << time_now() << "] " << RESET << l << endl;
        sleep_ms(350);
    }

    // Show fake prompt and accept a single safe command
    cout << MAG << "4d# " << RESET << flush;
    string cmd;
    getline(cin, cmd);
    if (cmd == "status") {
        cout << CYAN << "4D Kernel Status: All temporal slices nominal.\n"
             << "Uptime: 0 days, 0:00:12 (simulated)\n"
             << "Active workers: 16\n" << RESET;
    } else if (cmd == "dump artifact") {
        cout << YELLOW << "Artifact preview: " << random_hex(48) << RESET << endl;
    } else if (cmd.empty()) {
        cout << YELLOW << "(no-op) returning to host\n" << RESET;
    } else {
        cout << RED << cmd << ": command not found (simulation)\n" << RESET;
    }
}

// time helper
static string time_now() {
    auto t = system_clock::now();
    time_t tt = system_clock::to_time_t(t);
    struct tm buf;
#if defined(_WIN32)
    localtime_s(&buf, &tt);
#else
    localtime_r(&tt, &buf);
#endif
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%H:%M:%S", &buf);
    return string(tmp);
}

int main(int argc, char** argv) {
    cout << endl;
    run_simulation();
    cout << endl;
    return 0;
}
