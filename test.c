char bit_get(char* array, int index)
{
    int array_index = index / 8;
    int bit_index = index % 8;
    char set[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
    return array[array_index] & set[bit_index];
}

void main() {
	char s[1] = {0xff};
	int i;
	for (i = 0; i < 8; i++) {
		printf("%d\n", bit_get(s, i));
	}
}