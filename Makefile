all:
	touch src/main.rs
	mkdir -p target/debug/deps
	mkdir -p target/release/deps/
	cd ffi && make
	cargo build
