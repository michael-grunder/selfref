extern crate libc;
use libc::{c_int, c_void};
use std::cell::RefCell;
use std::rc::Rc;

#[repr(C)]
struct readTask {
    parent: *mut c_void,
    obj: *mut c_void,
}

#[repr(C)]
struct readHandlerFunctions {
    create_array: extern "C" fn(*const readTask, usize) -> *mut c_void,
    create_integer: extern "C" fn(*const readTask, c_int) -> *mut c_void,
}

#[repr(C)]
struct reader {
    fns: *mut readHandlerFunctions,
}

// Global struct with callbacks
const FNS: readHandlerFunctions = readHandlerFunctions {
    create_array: create_array,
    create_integer: create_integer,
};

// Rust reply container
#[derive(Debug)]
enum CustomReply {
    Integer(i32),
    Array(Vec<Rc<RefCell<CustomReply>>>),
}

impl CustomReply {
    fn integer(v: i32) -> Self {
        CustomReply::Integer(v)
    }

    fn array(size: usize) -> Self {
        CustomReply::Array(Vec::with_capacity(size))
    }
}

impl Drop for CustomReply {
    fn drop(&mut self) {
        println!("--- dropping ---");
        println!("{:?}", self);
    }
}

#[link(name = "ffitest", kind = "static")]
extern "C" {
    fn createReader(cb: *const readHandlerFunctions) -> *mut reader;
    fn getReply(reader: *mut reader, reply: *mut *mut c_void);
    fn freeReader(reader: *mut reader);
}

extern "C" fn create_array(task: *const readTask, len: usize) -> *mut c_void {
    let obj = CustomReply::array(len);
    attach_reply(task, obj)
}

extern "C" fn create_integer(task: *const readTask, v: c_int) -> *mut c_void {
    let obj = CustomReply::integer(v as i32);
    attach_reply(task, obj)
}

fn attach_reply(task: *const readTask, reply: CustomReply) -> *mut c_void {
    let rc = Rc::new(RefCell::new(reply));

    if unsafe { (*task).parent.is_null() != true } {
        let parent = unsafe { Rc::from_raw((*task).parent as *mut RefCell<CustomReply>) };
        {
            match *parent.borrow_mut() {
                CustomReply::Array(ref mut v) => v.push(rc.clone()),
                _ => panic!("Parent not an array type!"),
            }
        }

        std::mem::forget(parent);
    }

    Rc::into_raw(rc) as *mut _
}

fn main() {
    unsafe {
        let reader = createReader(&FNS);
        let mut reply = std::ptr::null::<c_void>() as *mut c_void;

        getReply(reader, &mut reply);

        let r = Rc::from_raw(reply as *mut RefCell<CustomReply>);
        println!("{:#?}", r);

        freeReader(reader);
    }
}
