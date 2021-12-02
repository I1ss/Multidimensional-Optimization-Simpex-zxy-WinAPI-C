#include <Windows.h>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

namespace work {
    inline double func_first(double x1, double x2) { return (1 - x1) * (1 - x1) + (2 - x2) * (2 - x2); }
    inline double func_second(double x1, double x2) { return x1 * x1 - 4 + (x2 + 6) * (x2 + 6); }

    double length(std::pair<double, double> x1, std::pair<double, double> x2) {
        return sqrt(pow(x1.first - x2.first, 2) + pow(x1.second - x2.second, 2));
    }

    int count(double eps) {
        int k = 1;
        while (eps < 1) {
            eps *= 10;
            k *= 10;
        }
        return k;
    }

    void simplex(double eps, double alpha, std::pair<double, double> x0, double(*func)(double, double), std::vector<std::pair<double, double>>& arr, std::vector<std::vector<std::pair<double, double>>>& array) {
        int N = 2, M = round(1.65 * N + 0.05 * N * N), x_f = 1, x_s = 1, x_t = 1, UN_eps = count(eps);
        double g1 = (sqrt(N + 1) + N - 1) / (N * sqrt(2)) * alpha, g2 = (sqrt(N + 1) - 1) / (N * sqrt(2)) * alpha;
        std::pair<double, double> x1(x0.first + g2, x0.second + g1), x2(x0.first + g1, x0.second + g2), x_1(0, 0), x_2(0, 0), x_c(0, 0);
        for (size_t i = 0; i < 2; i++) {
            array[0].push_back(x0);
            array[1].push_back(x1);
            array[2].push_back(x2);
            double temp = func(x0.first, x0.second) > func(x1.first, x1.second) ? (func(x0.first, x0.second) > func(x2.first, x2.second) ? func(x0.first, x0.second) : func(x2.first, x2.second)) : (func(x1.first, x1.second) > func(x2.first, x2.second)) ? func(x1.first, x1.second) : func(x2.first, x2.second);
            if (temp == func(x0.first, x0.second)) {
                x_f = 1;
                x_s++;
                x_t++;
                if (i == 0) {
                    x_1.first = x0.first;
                    x_1.second = x0.second;
                }
                else if (i == 1) {
                    x_2.first = x_1.first;
                    x_2.second = x_2.second;
                    x_1.first = x0.first;
                    x_1.second = x0.second;
                }
                x_c.first = (x1.first + x2.first) / 2;
                x_c.second = (x1.second + x2.second) / 2;
                x0.first = 2 * x_c.first - x0.first;
                x0.second = 2 * x_c.second - x0.second;
            }
            else if (temp == func(x1.first, x1.second)) {
                x_f++;
                x_s = 1;
                x_t++;
                if (i == 0) {
                    x_1.first = x1.first;
                    x_1.second = x1.second;
                }
                else if (i == 1) {
                    x_2.first = x_1.first;
                    x_2.second = x_2.second;
                    x_1.first = x1.first;
                    x_1.second = x1.second;
                }
                x_c.first = (x0.first + x2.first) / 2;
                x_c.second = (x0.second + x2.second) / 2;
                x1.first = 2 * x_c.first - x1.first;
                x1.second = 2 * x_c.second - x1.second;
            }
            else if (temp == func(x2.first, x2.second)) {
                x_f++;
                x_s++;
                x_t = 1;
                if (i == 0) {
                    x_1.first = x2.first;
                    x_1.second = x2.second;
                }
                else if (i == 1) {
                    x_2.first = x_1.first;
                    x_2.second = x_2.second;
                    x_1.first = x2.first;
                    x_1.second = x2.second;
                }
                x_c.first = (x1.first + x0.first) / 2;
                x_c.second = (x1.second + x0.second) / 2;
                x2.first = 2 * x_c.first - x2.first;
                x2.second = 2 * x_c.second - x2.second;
            }
        }
        while (fabs(func(x0.first, x0.second) - func(x1.first, x1.second)) > eps && fabs(func(x0.first, x0.second) - func(x2.first, x2.second)) > eps && fabs(func(x2.first, x2.second) - func(x1.first, x1.second)) > eps && (length(x0, x1) > eps)) {
            array[0].push_back(x0);
            array[1].push_back(x1);
            array[2].push_back(x2);
            double temp = func(x0.first, x0.second) > func(x1.first, x1.second) ? (func(x0.first, x0.second) > func(x2.first, x2.second) ? func(x0.first, x0.second) : func(x2.first, x2.second)) : (func(x1.first, x1.second) > func(x2.first, x2.second)) ? func(x1.first, x1.second) : func(x2.first, x2.second);
            if (x_f > M || x_s > M || x_t > M) {
                x_f = 1;
                x_s = 1;
                x_t = 1;
                alpha /= 2;
                g1 = (sqrt(N + 1) + N - 1) / (N * sqrt(2)) * alpha;
                g2 = (sqrt(N + 1) - 1) / (N * sqrt(2)) * alpha;
                temp = func(x0.first, x0.second) < func(x1.first, x1.second) ? (func(x0.first, x0.second) < func(x2.first, x2.second) ? func(x0.first, x0.second) : func(x2.first, x2.second)) : (func(x1.first, x1.second) < func(x2.first, x2.second)) ? func(x1.first, x1.second) : func(x2.first, x2.second);
                if (temp == func(x0.first, x0.second)) {
                    x1.first = x0.first + g2;
                    x1.second = x0.second + g1;
                    x2.first = x0.first + g1;
                    x2.second = x0.second + g2;
                }
                else if (temp == func(x1.first, x1.second)) {
                    x0.first = x1.first;
                    x0.second = x1.second;
                    x1.first = x0.first + g2;
                    x1.second = x0.second + g1;
                    x2.first = x0.first + g1;
                    x2.second = x0.second + g2;
                }
                else if (temp == func(x2.first, x2.second)) {
                    x0.first = x2.first;
                    x0.second = x2.second;
                    x1.first = x0.first + g2;
                    x1.second = x0.second + g1;
                    x2.first = x0.first + g1;
                    x2.second = x0.second + g2;
                }
                for (size_t i = 0; i < 2; i++) {
                    double temp = func(x0.first, x0.second) > func(x1.first, x1.second) ? (func(x0.first, x0.second) > func(x2.first, x2.second) ? func(x0.first, x0.second) : func(x2.first, x2.second)) : (func(x1.first, x1.second) > func(x2.first, x2.second)) ? func(x1.first, x1.second) : func(x2.first, x2.second);
                    if (temp == func(x0.first, x0.second)) {
                        x_f = 1;
                        x_s++;
                        x_t++;
                        if (i == 0) {
                            x_1.first = x0.first;
                            x_1.second = x0.second;
                        }
                        else if (i == 1) {
                            x_2.first = x_1.first;
                            x_2.second = x_2.second;
                            x_1.first = x0.first;
                            x_1.second = x0.second;
                        }
                        x_c.first = (x1.first + x2.first) / 2;
                        x_c.second = (x1.second + x2.second) / 2;
                        x0.first = 2 * x_c.first - x0.first;
                        x0.second = 2 * x_c.second - x0.second;
                    }
                    else if (temp == func(x1.first, x1.second)) {
                        x_f++;
                        x_s = 1;
                        x_t++;
                        if (i == 0) {
                            x_1.first = x1.first;
                            x_1.second = x1.second;
                        }
                        else if (i == 1) {
                            x_2.first = x_1.first;
                            x_2.second = x_2.second;
                            x_1.first = x1.first;
                            x_1.second = x1.second;
                        }
                        x_c.first = (x0.first + x2.first) / 2;
                        x_c.second = (x0.second + x2.second) / 2;
                        x1.first = 2 * x_c.first - x1.first;
                        x1.second = 2 * x_c.second - x1.second;
                    }
                    else if (temp == func(x2.first, x2.second)) {
                        x_f++;
                        x_s++;
                        x_t = 1;
                        if (i == 0) {
                            x_1.first = x2.first;
                            x_1.second = x2.second;
                        }
                        else if (i == 1) {
                            x_2.first = x_1.first;
                            x_2.second = x_2.second;
                            x_1.first = x2.first;
                            x_1.second = x2.second;
                        }
                        x_c.first = (x1.first + x0.first) / 2;
                        x_c.second = (x1.second + x0.second) / 2;
                        x2.first = 2 * x_c.first - x2.first;
                        x2.second = 2 * x_c.second - x2.second;
                    }
                }
            }
            else if (temp == func(x_2.first, x_2.second)) {
                if (temp == func(x0.first, x0.second)) {
                    temp = func(x1.first, x1.second) > func(x2.first, x2.second) ? func(x1.first, x1.second) : func(x2.first, x2.second);
                    if (temp == func(x1.first, x1.second)) {
                        x_f++;
                        x_s = 1;
                        x_t++;
                        x_2.first = x_1.first;
                        x_2.second = x_2.second;
                        x_1.first = x1.first;
                        x_1.second = x1.second;
                        x_c.first = (x0.first + x2.first) / 2;
                        x_c.second = (x0.second + x2.second) / 2;
                        x1.first = 2 * x_c.first - x1.first;
                        x1.second = 2 * x_c.second - x1.second;
                    }
                    else if (temp == func(x2.first, x2.second)) {
                        x_f++;
                        x_s++;
                        x_t = 1;
                        x_2.first = x_1.first;
                        x_2.second = x_2.second;
                        x_1.first = x2.first;
                        x_1.second = x2.second;
                        x_c.first = (x0.first + x1.first) / 2;
                        x_c.second = (x0.second + x1.second) / 2;
                        x2.first = 2 * x_c.first - x2.first;
                        x2.second = 2 * x_c.second - x2.second;
                    }
                }
                else if (temp == func(x1.first, x1.second)) {
                    temp = func(x0.first, x0.second) > func(x2.first, x2.second) ? func(x0.first, x0.second) : func(x2.first, x2.second);
                    if (temp == func(x0.first, x0.second)) {
                        x_f = 1;
                        x_s++;
                        x_t++;
                        x_2.first = x_1.first;
                        x_2.second = x_2.second;
                        x_1.first = x0.first;
                        x_1.second = x0.second;
                        x_c.first = (x1.first + x2.first) / 2;
                        x_c.second = (x1.second + x2.second) / 2;
                        x0.first = 2 * x_c.first - x0.first;
                        x0.second = 2 * x_c.second - x0.second;
                    }
                    else if (temp == func(x2.first, x2.second)) {
                        x_f++;
                        x_s++;
                        x_t = 1;
                        x_2.first = x_1.first;
                        x_2.second = x_2.second;
                        x_1.first = x2.first;
                        x_1.second = x2.second;
                        x_c.first = (x0.first + x1.first) / 2;
                        x_c.second = (x0.second + x1.second) / 2;
                        x2.first = 2 * x_c.first - x2.first;
                        x2.second = 2 * x_c.second - x2.second;
                    }
                }
                else if (temp == func(x2.first, x2.second)) {
                    temp = func(x1.first, x1.second) > func(x0.first, x0.second) ? func(x1.first, x1.second) : func(x0.first, x0.second);
                    if (temp == func(x1.first, x1.second)) {
                        x_f++;
                        x_s = 1;
                        x_t++;
                        x_2.first = x_1.first;
                        x_2.second = x_2.second;
                        x_1.first = x1.first;
                        x_1.second = x1.second;
                        x_c.first = (x0.first + x2.first) / 2;
                        x_c.second = (x0.second + x2.second) / 2;
                        x1.first = 2 * x_c.first - x1.first;
                        x1.second = 2 * x_c.second - x1.second;
                    }
                    else if (temp == func(x0.first, x0.second)) {
                        x_f = 1;
                        x_s++;
                        x_t++;
                        x_2.first = x_1.first;
                        x_2.second = x_2.second;
                        x_1.first = x0.first;
                        x_1.second = x0.second;
                        x_c.first = (x1.first + x2.first) / 2;
                        x_c.second = (x1.second + x2.second) / 2;
                        x0.first = 2 * x_c.first - x0.first;
                        x0.second = 2 * x_c.second - x0.second;
                    }
                }
            }
            else {
                if (temp == func(x0.first, x0.second)) {
                    x_f = 1;
                    x_s++;
                    x_t++;
                    x_2.first = x_1.first;
                    x_2.second = x_2.second;
                    x_1.first = x0.first;
                    x_1.second = x0.second;
                    x_c.first = (x1.first + x2.first) / 2;
                    x_c.second = (x1.second + x2.second) / 2;
                    x0.first = 2 * x_c.first - x0.first;
                    x0.second = 2 * x_c.second - x0.second;
                }
                else if (temp == func(x1.first, x1.second)) {
                    x_f++;
                    x_s = 1;
                    x_t++;
                    x_2.first = x_1.first;
                    x_2.second = x_2.second;
                    x_1.first = x1.first;
                    x_1.second = x1.second;
                    x_c.first = (x0.first + x2.first) / 2;
                    x_c.second = (x0.second + x2.second) / 2;
                    x1.first = 2 * x_c.first - x1.first;
                    x1.second = 2 * x_c.second - x1.second;
                }
                else if (temp == func(x2.first, x2.second)) {
                    x_f++;
                    x_s++;
                    x_t = 1;
                    x_2.first = x_1.first;
                    x_2.second = x_2.second;
                    x_1.first = x2.first;
                    x_1.second = x2.second;
                    x_c.first = (x0.first + x1.first) / 2;
                    x_c.second = (x0.second + x1.second) / 2;
                    x2.first = 2 * x_c.first - x2.first;
                    x2.second = 2 * x_c.second - x2.second;
                }
            }
            arr[0].first = round(x0.first * UN_eps) / UN_eps;
            arr[0].second = round(x0.second * UN_eps) / UN_eps;
            arr[1].first = round(x1.first * UN_eps) / UN_eps;
            arr[1].second = round(x1.second * UN_eps) / UN_eps;
            arr[2].first = round(x2.first * UN_eps) / UN_eps;
            arr[2].second = round(x2.second * UN_eps) / UN_eps;
        }
    }

    void call(std::vector<std::pair<double, double>>& arr, std::pair<double, double> x0, double alpha, double eps, double (*func)(double, double), std::vector<std::vector<std::pair<double, double>>>& array) {
        simplex(eps, alpha, x0, func, arr, array);
    }
}

void MyMenu(HWND hWnd) {
    HMENU hMainMenu = CreateMenu();
    HMENU hPopMenuFile = CreatePopupMenu();
    AppendMenu(hMainMenu, MF_STRING | MF_POPUP, (UINT)hPopMenuFile, L"Справка");
    AppendMenu(hPopMenuFile, MF_STRING, 1, L"Информация о методе");
    AppendMenu(hPopMenuFile, MF_SEPARATOR, 0, L"");
    AppendMenu(hPopMenuFile, MF_STRING, 2, L"Автор данного приложения: Lis.");
    SetMenu(hWnd, hMainMenu);
    SetMenu(hWnd, hPopMenuFile);
}

class MyApp {
private:
    HWND a_hWnd, static_temp_F, static_temp_S, * e_hWnds, * b_hWnds, * s_hWnds;
    std::vector<std::vector<std::pair<double, double>>> array;
    int const width = 800, heigth = 800;
    bool check = false;
    void create_wnd(LPCWSTR name) {
        WNDCLASSEX wc{ sizeof(WNDCLASSEX) };
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = nullptr;
        wc.hIcon = LoadIcon(nullptr, IDI_SHIELD);
        wc.hIconSm = wc.hIcon;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = reinterpret_cast<HBRUSH>(CreateSolidBrush(RGB(141, 141, 141)));
        wc.lpszMenuName = nullptr;
        wc.lpszClassName = name;
        wc.style = CS_VREDRAW | CS_HREDRAW;
        wc.lpfnWndProc = get_ptr;
        if (!RegisterClassEx(&wc)) {
            throw "Smth bad!";
        }
        RECT wnd_r{ 0, 0, width, heigth };
        AdjustWindowRect(&wnd_r, WS_OVERLAPPEDWINDOW, FALSE);
        a_hWnd = CreateWindow(name, L"Многомерная оптимизация.", WS_SYSMENU | WS_DLGFRAME | WS_MINIMIZEBOX, (GetSystemMetrics(SM_CXSCREEN) - wnd_r.right) / 2,
            (GetSystemMetrics(SM_CYSCREEN) - wnd_r.bottom) / 2, width, heigth, nullptr, nullptr, nullptr, this);
        MyMenu(a_hWnd);
        create_native_smth();
    }
    static LRESULT CALLBACK get_ptr(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        static MyApp* self = nullptr;
        if (uMsg == WM_CREATE) {
            self = (MyApp*)((CREATESTRUCT*)lParam)->lpCreateParams;
            self->a_hWnd = hWnd;
        }
        if (self == nullptr) {
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
        return self->wnd_proc(hWnd, uMsg, wParam, lParam);
    }
    LRESULT CALLBACK wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        RECT wnd_r{ 251, 526, 549, 724 };
        if (uMsg == WM_DESTROY) {
            PostQuitMessage(EXIT_FAILURE);
        }
        else if (uMsg == WM_PAINT) {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(a_hWnd, &ps);
            Rectangle(hdc, 250, 525, 550, 725);
            FillRect(hdc, &wnd_r, reinterpret_cast<HBRUSH>(CreateSolidBrush(RGB(221, 221, 221))));
            MoveToEx(hdc, 250, 625, nullptr);
            LineTo(hdc, 550, 625);
            MoveToEx(hdc, 400, 525, nullptr);
            LineTo(hdc, 400, 725);
            MoveToEx(hdc, 400, 625, nullptr);
            SetViewportOrgEx(hdc, 400, 625, nullptr);
            double correction = 10;
            for (int i = 0; i < array[0].size(); i++) {
                if (i == 0) {
                    if (fabs(array[0][array[0].size() - 1].first) > 10) {
                        correction /= fabs((array[0][array[0].size() - 1].first)) / correction;
                    }
                    else if (fabs(array[0][array[0].size() - 1].second) > 10) {
                        correction /= fabs((array[0][array[0].size() - 1].second)) / correction;
                    }
                }
                MoveToEx(hdc, correction * array[0][i].first, -correction * array[0][i].second, nullptr);
                LineTo(hdc, correction * array[1][i].first, -correction * array[1][i].second);
                LineTo(hdc, correction * array[2][i].first, -correction * array[2][i].second);
                LineTo(hdc, correction * array[0][i].first, -correction * array[0][i].second);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            static_temp_F = CreateWindow(L"static", std::to_wstring(100 / correction).c_str(), WS_CHILD | WS_VISIBLE | WS_BORDER, 340, 530, 38, 20, a_hWnd, nullptr, nullptr, nullptr);
            static_temp_S = CreateWindow(L"static", std::to_wstring(100 / correction).c_str(), WS_CHILD | WS_VISIBLE | WS_BORDER, 490, 630, 38, 20, a_hWnd, nullptr, nullptr, nullptr);
            EndPaint(a_hWnd, &ps);
        }
        else if (uMsg == WM_COMMAND) {
            if (LOWORD(wParam) == 1) {
                MessageBox(nullptr, L"Цель симплекс-метода -- это многомерная оптимизация некоторой функции, зависящей от двух и более переменных. Данное приложение оптимизирует функции вида: z(x,y).", L"Информация", MB_OK);
            }
            else if (LOWORD(wParam) == 2) {
                MessageBox(nullptr, L"lebedewgeorg@yandex.ru", L"Обратная связь", MB_OK);
            }
            else if (LOWORD(wParam) == 5) {
                array[0].clear();
                array[1].clear();
                array[2].clear();
                double (*func)(double, double);
                std::wstring x0f, x0s, alpha, eps, new_s = L"Координаты конечного симплекса: ", new_ss = L"f(x1,x2)min = ";
                x0f.resize(MAX_PATH);
                x0s.resize(MAX_PATH);
                alpha.resize(MAX_PATH);
                eps.resize(MAX_PATH);
                GetWindowText(e_hWnds[0], &x0f[0], MAX_PATH);
                GetWindowText(e_hWnds[1], &x0s[0], MAX_PATH);
                GetWindowText(e_hWnds[2], &alpha[0], MAX_PATH);
                GetWindowText(e_hWnds[3], &eps[0], MAX_PATH);
                std::vector<std::pair<double, double>> arr(3);
                if (IsDlgButtonChecked(a_hWnd, 3)) {
                    func = work::func_first;
                    work::call(arr, std::pair<double, double>(std::stod(x0f), std::stod(x0s)), std::stod(alpha), std::stod(eps), func, array);
                    check = true;
                }
                else if (IsDlgButtonChecked(a_hWnd, 4)) {
                    func = work::func_second;
                    work::call(arr, std::pair<double, double>(std::stod(x0f), std::stod(x0s)), std::stod(alpha), std::stod(eps), func, array);
                }
                else {
                    MessageBox(nullptr, L"You should choose func!", L"Error.", MB_OK);
                    return DefWindowProc(hWnd, uMsg, wParam, lParam);
                }
                for (size_t i = 0; i < 3; i++) {
                    new_s += std::to_wstring(arr[i].first) + L"; " + std::to_wstring(arr[i].second) + L"; ";
                    new_ss += std::to_wstring(func(arr[i].first, arr[i].second)) + L"; ";
                }
                SetWindowText(s_hWnds[5], new_s.c_str());
                SetWindowText(s_hWnds[6], new_ss.c_str());
                InvalidateRect(a_hWnd, &wnd_r, FALSE);
            }
        }
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    void create_native_smth() {
        b_hWnds = new HWND[3];
        e_hWnds = new HWND[4];
        s_hWnds = new HWND[7];
        HFONT hFont = CreateFont(32, 0, 0, 0, 0, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, L"Roboto");
        HFONT hFontMin = CreateFont(21, 0, 0, 0, 0, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, L"Roboto");
        s_hWnds[0] = CreateWindow(L"static", L"Выберите функцию для оптимизации:", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER, 100, 35, 600, 35, a_hWnd, nullptr, nullptr, nullptr);
        b_hWnds[0] = CreateWindow(L"button", L"1) f(x1, x2) = (1-x1)^2 + (2-x2)^2;", WS_CHILD | WS_VISIBLE | WS_BORDER | BS_AUTORADIOBUTTON, 110, 100, 275, 35, a_hWnd, reinterpret_cast<HMENU>(3), nullptr, nullptr);
        b_hWnds[1] = CreateWindow(L"button", L"2) f(x1, x2) = x1^2-4 + (x2+6)^2;", WS_CHILD | WS_VISIBLE | WS_BORDER | BS_AUTORADIOBUTTON, 415, 100, 275, 35, a_hWnd, reinterpret_cast<HMENU>(4), nullptr, nullptr);
        s_hWnds[1] = CreateWindow(L"static", L"Введите условия задачи:", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER, 100, 170, 600, 35, a_hWnd, nullptr, nullptr, nullptr);
        s_hWnds[2] = CreateWindow(L"static", L"x0( ...... ; ...... );", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER, 80, 240, 220, 35, a_hWnd, nullptr, nullptr, nullptr);
        e_hWnds[0] = CreateWindow(L"edit", L"0", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 150, 247, 45, 24, a_hWnd, nullptr, nullptr, nullptr);
        e_hWnds[1] = CreateWindow(L"edit", L"0", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 210, 247, 45, 24, a_hWnd, nullptr, nullptr, nullptr);
        s_hWnds[3] = CreateWindow(L"static", L"alpha = ......;", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER, 340, 240, 170, 35, a_hWnd, nullptr, nullptr, nullptr);
        e_hWnds[2] = CreateWindow(L"edit", L"2", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 445, 247, 45, 24, a_hWnd, nullptr, nullptr, nullptr);
        s_hWnds[4] = CreateWindow(L"static", L"eps = .........;", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER, 550, 240, 170, 35, a_hWnd, nullptr, nullptr, nullptr);
        e_hWnds[3] = CreateWindow(L"edit", L"0.0001", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 635, 247, 65, 24, a_hWnd, nullptr, nullptr, nullptr);
        b_hWnds[2] = CreateWindow(L"button", L"Оптимизировать.", WS_CHILD | WS_BORDER | WS_VISIBLE, 250, 450, 300, 40, a_hWnd, reinterpret_cast<HMENU>(5), nullptr, nullptr);
        s_hWnds[5] = CreateWindow(L"static", L"Координаты конечного симплекса:", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER, 50, 300, 700, 70, a_hWnd, nullptr, nullptr, nullptr);
        s_hWnds[6] = CreateWindow(L"static", L"f(x1,x2)min = ", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER, 50, 390, 700, 35, a_hWnd, nullptr, nullptr, nullptr);
        for (size_t i = 0; i < 7; i++) {
            SendMessage(s_hWnds[i], WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);
            if (i < 3) {
                SendMessage(b_hWnds[i], WM_SETFONT, reinterpret_cast<WPARAM>(hFontMin), TRUE);
            }
            if (i < 4) {
                SendMessage(e_hWnds[i], WM_SETFONT, reinterpret_cast<WPARAM>(hFontMin), TRUE);
            }
        }
        SendMessage(b_hWnds[0], BM_SETCHECK, BST_CHECKED, FALSE);
    }
    int work() {
        MSG msg;
        ShowWindow(a_hWnd, SW_SHOWDEFAULT);
        UpdateWindow(a_hWnd);
        while (GetMessage(&msg, nullptr, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return static_cast<int>(msg.wParam);
    }
public:
    MyApp() {
        array.resize(3);
        std::wstring name = std::to_wstring(rand() % 10000 - 10000);
        create_wnd(name.c_str());
        work();
    }
    ~MyApp() {
        if (s_hWnds == nullptr || e_hWnds == nullptr || b_hWnds == nullptr) {
            throw "Smth bad with memory.";
        }
        else {
            delete[] s_hWnds;
            delete[] e_hWnds;
            delete[] b_hWnds;
        }
    }
};

int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR lpszCmdLine, int nCmdShow) {
    MyApp temp;
    return 0;
}