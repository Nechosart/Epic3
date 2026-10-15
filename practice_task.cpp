// Аналізатор місячного бюджету
#include <iostream>
#include <iomanip>

using namespace std;

void get_min_max(float spending[], int N, float* min_spending, float* max_spending) {
	*min_spending = spending[0];
	for (int i = 1; i < N; i++) {
		if (spending[i] < *min_spending) {
			*min_spending = spending[i];
		}
		if (spending[i] > *max_spending) {
			*max_spending = spending[i];
		}
	}
}

float calc_sum_spending(float spending[], int i) {
	if (i == 0) {
		return spending[i];
	} else {
		return spending[i] + calc_sum_spending(spending, i-1);
	}
}

int main() {

	cout << "Бюджет на місяць (грн): ";
	float budget;
	cin >> budget;

	cout << "Кількість днів обліку: ";
	int N;
	cin >> N;

	const int max_days = 31;
	float spending[max_days];
	cout << "Витрати по днях (грн): ";
	for (int i = 0; i < N; i++) {
		cin >> spending[i];
	}

	int action;
	bool is_working = true;
	float min_spending, max_spending, sum_spending, over_spending, average_spending, day_limit;
	int days_with_buys, days_with_over_limit;
	const float day_limit_default = 1000;

	const int top_spending_count = 3;
	float spending_sorted[max_days];
	int top_spending_possible;

	// Меню
	menu:
		do {

			cout << "1 - Показати витрати по днях" << endl 
				<< "2 - Статистика витрат" << endl
				<< "3 - Дні з перевищенням денного ліміту" << endl
				<< "4 - Три найдорожчі дні" << endl
				<< "0 - Вийти" << endl;

			cin >> action;

			if (cin.fail()) {
				cin.clear();
				cin.ignore(256, '\n');
				cout << "Помилка вводу. Спробуйте ще раз." << endl;
				goto menu;
			}

			switch (action) {
				case 1:
					cout << left << setw(10) << "День" 
						<< right << setw(10) << "Витрати, грн" << endl;

					for (int i = 0; i < N; i++) {
						cout << left << setw(10) << "День " << (i + 1) << ": " 
							<< right << setw(10) << spending[i] << " грн" << endl;
					}

					break;

				case 2: {
							get_min_max(spending, N, &min_spending, &max_spending);

							cout << left << setw(32) << "Мінімальні витрати за день: "
								<< right << setw(16) << min_spending << " грн" << endl
								<< left << setw(32) << "Максимальні витрати за день: "
								<< right << setw(16) << max_spending << " грн" << endl;

							sum_spending = calc_sum_spending(spending, N - 1);

							cout << left << setw(32) << "Сумарні витрати: "
								<< right << setw(16) << sum_spending << " грн" << endl;

							if (sum_spending > budget) {
								over_spending = abs(budget - sum_spending);
								cout << left << setw(32) << "Перевитрата понад бюджет: "
									<< right << setw(16) << over_spending << " грн" << endl;
							}

							days_with_buys = 0;
							for (int i = 0; i < N; i++) {
								if (spending[i] == 0) {
									continue;
								}
								days_with_buys++;
							}

							if (sum_spending > 0) {
								average_spending = sum_spending / days_with_buys;
							}
							else {
								average_spending = 0;
							}

							cout << left << setw(32) << "Днів з покупками: "
								<< right << setw(16) << days_with_buys << " грн" << endl
								<< left << setw(32) << "Середні витрати за такий день: "
								<< right << setw(16) << average_spending << " грн" << endl;

							int i = 0;
							sum_spending = 0;
							while (true) {
								sum_spending += spending[i];

								if (sum_spending > budget) {
									cout << "Бюджет вичерпано на дні " << i << ", накопичено: " << sum_spending << " грн." << endl;
									break;
								}

								i++;
								if (i == N) {
									cout << "Бюджету вистачило на всі дні, залишок: " << budget - sum_spending << " грн." << endl;
									break;
								}
							}

							break;
						}
				case 3:
					cout << "Денний ліміт (грн, 0 = за замовчуванням): " << endl;

					cin >> day_limit;

					if (cin.fail()) {
						cin.clear();
						cin.ignore(256, '\n');
						cout << "Помилка вводу. Спробуйте ще раз." << endl;
						goto menu;
					}

					if (day_limit == 0) {
						day_limit = day_limit_default;
					}
					cout << "Ліміт: " << day_limit << " грн" << endl;

					cout << "Дні з перевищенням ліміту: " << endl;
					days_with_over_limit = 0;
					for (int i = 0; i < N; i++) {
						if (spending[i] > day_limit) {
							cout << left << setw(12) << "  день " << i << ":" 
								<< right << setw(12) << spending[i] << " грн" << endl;
							days_with_over_limit++;
						}
					}
					cout << "Разом днів з перевищенням: " << days_with_over_limit << endl;

					break;

				case 4:
					for (int i = 0; i < N; i++) {
						spending_sorted[i] = spending[i];

						for (int j = i - 1; j >= 0; j--) {
							if (spending_sorted[j] < spending_sorted[j+1]) {
								float temp = spending_sorted[j];
								spending_sorted[j] = spending_sorted[j+1];
								spending_sorted[j+1] = temp;
							}
						}
					}

					if (N > top_spending_count) {
						top_spending_possible = top_spending_count;
					} else {
						top_spending_possible = N;
					}

					cout << "Топ-" << top_spending_possible << " витрат: ";
					for (int i = 0; i < top_spending_possible; i++) {
						cout << spending_sorted[i] << ", ";
					}
					cout << "грн" << endl;

					break;

				case 0:
					is_working = false;
			}

		} while (is_working);

	return 0;
}
