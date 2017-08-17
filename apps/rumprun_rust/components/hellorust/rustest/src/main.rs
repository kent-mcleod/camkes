#![feature(linkage)]

fn main() {
    println!("Hello rust!");
    unsafe {
        (*((&camkes_ev_emit as *const *const extern "C" fn()) as *const extern "C" fn()))();
    }
    println!("Waiting for event!");
    unsafe {
        std::mem::transmute::<*const extern "C" fn(), extern "C" fn()>(camkes_ev1_wait)();
    }
    println!("Got event!");

}

#[no_mangle]
extern "C" {
    #[linkage = "extern_weak"]
    static camkes_ev_emit: *const extern "C" fn();
    #[linkage = "extern_weak"]
    static camkes_ev1_wait: *const extern "C" fn();
}
