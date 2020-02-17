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
								
								int *in = new int[n];
									int *out = new int[n];
										for (int i = 0; i < n; i++) {
														in[i] = 0;
																out[i] = 0;
																	}
											
											for (int i = 0; i < n; i++) {
															for (int j = 0; j < n; j++)
																				if (a[i][j] == 1) {
																										out[i]++;
																														in[j]++;
																																	}
																}
												for (int i = 0; i < n; i++) {
																cout << in[i] << endl;
																		cout << out[i] << endl;
																			}
}

