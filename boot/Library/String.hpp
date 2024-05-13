#ifndef strlen

unsigned long long strlen(const char *str) {
  const char* s;
  for (s=str;*s;++s);
  return (s-str);
}

#endif

#ifndef strcmp

unsigned long long strcmp(unsigned char* a, unsigned char* b, unsigned long long length) {
	for (unsigned long long i=0;i<length;i++) if (*a != *b) return 0;
	return 1;
}

#endif