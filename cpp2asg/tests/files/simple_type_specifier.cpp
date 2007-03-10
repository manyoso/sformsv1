
void Foo::bar() {
  do {
    int i = 0;
    for (; i < 10; ++i)
      bar();
  } while(true);
}

