use std::process::Command;
use std::env;
use std::path::Path;

fn main() {
    let out_dir = env::var("OUT_DIR").unwrap();

    Command::new("ar").args(&["crus", "libhello.a", "/home/podhrad/Workspace/sel4/projects/camkes-apps-test/build/x86/pc99/rumprun_rust/src/hello1/generated/camkes.o"])
                      .current_dir(&Path::new(&out_dir))
                      .status().unwrap();

    println!("cargo:rustc-link-search=native={}", out_dir);
    println!("cargo:rustc-link-lib=static=hello");
}