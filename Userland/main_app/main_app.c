void window1();
void window2();
void initWindow1();
void initWindow2();

int main(){

	initWindow1();
	initWindow2();

	while(1){
		window1();
		window2();
	}
	
	return 1;

}