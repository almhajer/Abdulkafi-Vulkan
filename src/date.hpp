//
// Created by mac on 11/09/2024.
//

#ifndef ABDULKAFI_DATE_HPP
#define ABDULKAFI_DATE_HPP
#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <string>

/**
 * @brief ترجم اليوم من الرقم إلى الاسم باللغة العربية.
 * @param localTime هيكل tm يحتوي على المعلومات الزمنية مثل اليوم والشهر والسنة.
 * @return اسم اليوم باللغة العربية.
 */
std::string translateDay(const std::tm* localTime) {
    switch (localTime->tm_wday) {
        case 0: return "الأحد";
        case 1: return "الاثنين";
        case 2: return "الثلاثاء";
        case 3: return "الأربعاء";
        case 4: return "الخميس";
        case 5: return "الجمعة";
        case 6: return "السبت";
        default: return "";
    }
}

/**
 * @brief ترجم الشهر من الرقم إلى الاسم باللغة العربية.
 * @param localTime هيكل tm يحتوي على المعلومات الزمنية مثل اليوم والشهر والسنة.
 * @return اسم الشهر باللغة العربية.
 */
std::string translateMonth(const std::tm* localTime) {
    switch (localTime->tm_mon) {
        case 0: return "يناير";
        case 1: return "فبراير";
        case 2: return "مارس";
        case 3: return "أبريل";
        case 4: return "مايو";
        case 5: return "يونيو";
        case 6: return "يوليو";
        case 7: return "أغسطس";
        case 8: return "سبتمبر";
        case 9: return "أكتوبر";
        case 10: return "نوفمبر";
        case 11: return "ديسمبر";
        default: return "";
    }
}

/**
 * @brief طباعة التاريخ الحالي باللغة العربية مع ترجمة اليوم والشهر.
 * @return لا يوجد قيمة مرجعة (void).
 */
void printCurrentDateInArabic() {
    // الحصول على الوقت الحالي كنقطة زمنية
    auto now = std::chrono::system_clock::now();

    // تحويل الوقت إلى time_t
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    // تحويل time_t إلى هيكل tm
    std::tm* localTime = std::localtime(&currentTime);

    // طباعة اليوم والتاريخ بالتنسيق اليدوي
    std::cout << "التاريخ الحالي: "
              << translateDay(localTime) << ", "   // ترجمة اليوم
              << localTime->tm_mday << " "         // اليوم من الشهر
              << translateMonth(localTime) << " "  // ترجمة الشهر
              << (1900 + localTime->tm_year)       // السنة
              << std::endl;
}

//int main() {
//    // استدعاء الدالة لطباعة التاريخ الحالي باللغة العربية
//    printCurrentDateInArabic();
//
//    return 0;
//}


#endif //ABDULKAFI_DATE_HPP
