
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>
#include <algorithm>

using namespace std;

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int n, choice;
    string fileName = "image.bmp";

    cout << "Введіть розмір зображення: ";
    cin >> n;

    if (n < 10 || n > 2000)
    {
        cout << "Неправильний розмір!";
        return 1;
    }

    cout << "\nВиберіть фігуру:\n";
    cout << "1 - Діагональний трикутник зліва\n";
    cout << "2 - Трикутник зверху\n";
    cout << "3 - Діагональний трикутник справа\n";
    cout << "4 - Чотири трикутники\n";
    cout << "5 - Трикутник знизу\n";
    cout << "Ваш вибір: ";
    cin >> choice;

    if (choice < 1 || choice > 5)
    {
        cout << "Неправильний вибір!";
        return 1;
    }

    int light, dark;

    cout << "Колір світлої частини (0-255): ";
    cin >> light;

    cout << "Колір темної частини (0-255): ";
    cin >> dark;

    if (light < 0 || light > 255 ||
        dark < 0 || dark > 255)
    {
        cout << "Неправильний колір!";
        return 1;
    }

    vector<vector<unsigned char>> pixels(
        n, vector<unsigned char>(n, (unsigned char)light));

    for (int y = 0; y < n; y++)
    {
        for (int x = 0; x < n; x++)
        {
            bool black = false;

            switch (choice)
            {
            case 1:
                black = (x <= y);
                break;

            case 2:
                black = (y <= n / 2 &&
                    abs(x - n / 2) <= y);
                break;

            case 3:
                black = (x >= y);
                break;

            case 4:
                black = (abs(x - n / 2) >=
                    abs(y - n / 2));
                break;

            case 5:
                black = (y >= n / 2 &&
                    abs(x - n / 2) <= y - n / 2);
                break;
            }

            if (black)
                pixels[y][x] = (unsigned char)dark;
        }
    }

    ofstream file(fileName, ios::binary);

    if (!file)
    {
        cout << "Не вдалося створити файл!";
        return 1;
    }

    int rowSize = (n * 3 + 3) & (~3);
    int dataSize = rowSize * n;
    int fileSize = 54 + dataSize;

    unsigned char header[54] = {};

    header[0] = 'B';
    header[1] = 'M';

    *reinterpret_cast<int*>(&header[2]) = fileSize;
    *reinterpret_cast<int*>(&header[10]) = 54;
    *reinterpret_cast<int*>(&header[14]) = 40;
    *reinterpret_cast<int*>(&header[18]) = n;
    *reinterpret_cast<int*>(&header[22]) = n;

    header[26] = 1;
    header[28] = 24;

    *reinterpret_cast<int*>(&header[34]) = dataSize;

    file.write(reinterpret_cast<char*>(header), 54);

    vector<unsigned char> row(rowSize, 0);

    for (int y = n - 1; y >= 0; y--)
    {
        for (int x = 0; x < n; x++)
        {
            unsigned char color = pixels[y][x];

            row[x * 3] = color;
            row[x * 3 + 1] = color;
            row[x * 3 + 2] = color;
        }

        file.write(reinterpret_cast<char*>(row.data()),
            rowSize);
    }

    file.close();

    cout << "\nЗображення успішно збережено у файлі "
        << fileName << endl;

    return 0;
}
