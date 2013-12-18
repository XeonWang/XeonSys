void main() {
	char *video_memory = (char*) 0xB8000;
	*video_memory = 'H';
	*(video_memory+2) = 'e';
	*(video_memory+4) = 'l';
	*(video_memory+6) = 'l';
	*(video_memory+8) = 'o';
}
