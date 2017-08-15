fn main() {
 println!("Hello rust!");
 unsafe {
 	camkes_ev_emit();
 }
 println!("Waiting for event!");
 unsafe {
	 camkes_ev1_wait();
 }
 println!("Got event!");
 
}

#[no_mangle]
#[link(name = "hello1.instance")]
extern {
    fn camkes_ev_emit();
    fn camkes_ev1_wait(); 
}