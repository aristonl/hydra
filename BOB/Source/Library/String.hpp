#ifndef strlen

unsigned long long strlen(const char *str) {
  const char* s;
  for (s=str;*s;++s);
  return (s-str);
}

#endif