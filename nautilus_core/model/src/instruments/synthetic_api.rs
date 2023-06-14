// -------------------------------------------------------------------------------------------------
//  Copyright (C) 2015-2023 Nautech Systems Pty Ltd. All rights reserved.
//  https://nautechsystems.io
//
//  Licensed under the GNU Lesser General Public License Version 3.0 (the "License");
//  You may not use this file except in compliance with the License.
//  You may obtain a copy of the License at https://www.gnu.org/licenses/lgpl-3.0.en.html
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
// -------------------------------------------------------------------------------------------------

use std::{
    ffi::c_char,
    ops::{Deref, DerefMut},
    str::FromStr,
};

use nautilus_core::{cvec::CVec, parsing::bytes_to_string_vec, string::cstr_to_string};

use super::synthetic::SyntheticInstrument;
use crate::{
    identifiers::{instrument_id::InstrumentId, symbol::Symbol},
    types::price::Price,
};

/// Provides a C compatible Foreign Function Interface (FFI) for an underlying
/// [`SyntheticInstrument`].
///
/// This struct wraps `SyntheticInstrument` in a way that makes it compatible with C function
/// calls, enabling interaction with `SyntheticInstrument` in a C environment.
///
/// It implements the `Deref` trait, allowing instances of `SyntheticInstrument_API` to be
/// dereferenced to `SyntheticInstrument`, providing access to `SyntheticInstruments`'s methods without
/// having to manually access the underlying instance.
#[allow(non_camel_case_types)]
#[repr(C)]
pub struct SyntheticInstrument_API(Box<SyntheticInstrument>);

impl Deref for SyntheticInstrument_API {
    type Target = SyntheticInstrument;

    fn deref(&self) -> &Self::Target {
        &self.0
    }
}

impl DerefMut for SyntheticInstrument_API {
    fn deref_mut(&mut self) -> &mut Self::Target {
        &mut self.0
    }
}

/// # Safety
///
/// - Assumes `components_ptr` is a valid C string pointer of a JSON format list of strings.
/// - Assumes `formula_ptr` is a valid C string pointer.
#[no_mangle]
pub unsafe extern "C" fn synthetic_instrument_new(
    symbol: Symbol,
    precision: u8,
    components_ptr: *const c_char,
    formula_ptr: *const c_char,
) -> SyntheticInstrument_API {
    // TODO: There is absolutely no error handling here yet
    let components = bytes_to_string_vec(components_ptr)
        .into_iter()
        .map(|s| InstrumentId::from_str(&s).unwrap())
        .collect::<Vec<InstrumentId>>();
    let formula = cstr_to_string(formula_ptr);
    let synth = SyntheticInstrument::new(symbol, precision, components, formula).unwrap();

    SyntheticInstrument_API(Box::new(synth))
}

#[no_mangle]
pub extern "C" fn synthetic_instrument_drop(synth: SyntheticInstrument_API) {
    drop(synth); // Memory freed here
}

#[no_mangle]
pub extern "C" fn synthetic_instrument_precision(synth: &SyntheticInstrument_API) -> u8 {
    synth.precision
}

#[no_mangle]
pub extern "C" fn synthetic_instrument_calculate(
    synth: &SyntheticInstrument_API,
    inputs_ptr: CVec,
) -> Price {
    let CVec { ptr, len, cap } = inputs_ptr;
    let inputs: Vec<f64> = unsafe { Vec::from_raw_parts(ptr as *mut f64, len, cap) };

    // TODO: There is absolutely no error handling here yet
    synth.calculate(inputs.as_slice()).unwrap()
}
