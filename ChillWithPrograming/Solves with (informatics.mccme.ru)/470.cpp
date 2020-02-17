nclude <iostream>

using namespace std;

int main() {
			int n;
				cin >> n;
					int** a = new int*[n];
						for (int i = 0; i < n; i++)
											a[i] = new int[n];

							for (int i = 0; i < n; i++)
											for (int j = 0; j < n; j++)
																		cin >> a[i][j];

								int* res = new int[n];
									for (int i = 0; i < n; i++) {
													int tmp = 0;
															for (int j = 0; j < n; j++)
																				if (a[i][j] == 1) tmp++;
																	res[i] = tmp;
																		}
										for (int i = 0; i < n; i++)
															cout << res[i] << endl;
}

