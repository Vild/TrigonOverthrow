function format() { find src/ -path ./src/lib -iname "*.[ch]pp" -exec clang-format -i {} \; }
function lint() {
	CFLAGS=$(grep "CFLAGS =" reggaefile.d | awk -F'"' '{print $2}')
	find src/ \! -path "src/lib/*" -name "*.cpp" -exec clang-tidy "-checks=modernize-*" -p . --header-filter=src/ {}  -- $CFLAGS \;
}
#clang-analyzer-*,readability-*
