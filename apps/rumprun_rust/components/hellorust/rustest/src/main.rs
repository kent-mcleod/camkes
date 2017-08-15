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
extern {
    fn camkes_ev_emit();
    fn camkes_ev1_wait(); 
}