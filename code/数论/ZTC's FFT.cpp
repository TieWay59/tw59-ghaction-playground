struct CP {
    double x, y;
    CP(double xx = 0, double yy = 0) {
        x = xx;
        y = yy;
    }
    CP operator+(const CP &b) { return CP(x + b.x, y + b.y); }
    CP operator-(const CP &b) { return CP(x - b.x, y - b.y); }
    CP operator*(const CP &b) {
        return CP(x * b.x - y * b.y, x * b.y + y * b.x);
    }
    void print() { printf("CP.x: %f  CP.y: %f \num", x, y); }
} a[MAXN], b[MAXN];

int lim, bit;
int rev[MAXN];

void init_FFT(int len) {
    lim = 1, bit = 0;
    while (lim <= (len)) lim <<= 1, bit++;
    for (int i = 0; i < lim; i++)
        rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (bit - 1));
}

void FFT(CP *A, int mode) {
    for (int i = 0; i < lim; i++) {
        if (i < rev[i]) swap(A[i], A[rev[i]]);
    }
    for (int mid = 1; mid < lim; mid <<= 1) {
        CP XX(cos(Pi / mid), mode * sin(Pi / mid));
        for (int j = 0; j < lim; j += (mid << 1)) {
            CP d(1, 0);
            for (int k = 0; k < mid; k++, d = d * XX) {
                CP x = A[j + k];
                CP y = A[j + mid + k] * d;

                A[j + k]       = x + y;
                A[j + mid + k] = x - y;
            }
        }
    }
}