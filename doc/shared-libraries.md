Shared Libraries
================

## scrytconsensus

The purpose of this library is to make the verification functionality that is critical to Scryt's consensus available to other applications, e.g. to language bindings.

### API

The interface is defined in the C header `scrytconsensus.h` located in  `src/script/scrytconsensus.h`.

#### Version

`scrytconsensus_version` returns an `unsigned int` with the API version *(currently at an experimental `0`)*.

#### Script Validation

`scrytconsensus_verify_script` returns an `int` with the status of the verification. It will be `1` if the input script correctly spends the previous output `scriptPubKey`.

##### Parameters
- `const unsigned char *scriptPubKey` - The previous output script that encumbers spending.
- `unsigned int scriptPubKeyLen` - The number of bytes for the `scriptPubKey`.
- `const unsigned char *txTo` - The transaction with the input that is spending the previous output.
- `unsigned int txToLen` - The number of bytes for the `txTo`.
- `unsigned int nIn` - The index of the input in `txTo` that spends the `scriptPubKey`.
- `unsigned int flags` - The script validation flags *(see below)*.
- `scrytconsensus_error* err` - Will have the error/success code for the operation *(see below)*.

##### Script Flags
- `scrytconsensus_SCRIPT_FLAGS_VERIFY_NONE`
- `scrytconsensus_SCRIPT_FLAGS_VERIFY_P2SH` - Evaluate P2SH ([BIP16](https://github.com/scryt/bips/blob/master/bip-0016.mediawiki)) subscripts
- `scrytconsensus_SCRIPT_FLAGS_VERIFY_DERSIG` - Enforce strict DER ([BIP66](https://github.com/scryt/bips/blob/master/bip-0066.mediawiki)) compliance
- `scrytconsensus_SCRIPT_FLAGS_VERIFY_NULLDUMMY` - Enforce NULLDUMMY ([BIP147](https://github.com/scryt/bips/blob/master/bip-0147.mediawiki))
- `scrytconsensus_SCRIPT_FLAGS_VERIFY_CHECKLOCKTIMEVERIFY` - Enable CHECKLOCKTIMEVERIFY ([BIP65](https://github.com/scryt/bips/blob/master/bip-0065.mediawiki))
- `scrytconsensus_SCRIPT_FLAGS_VERIFY_CHECKSEQUENCEVERIFY` - Enable CHECKSEQUENCEVERIFY ([BIP112](https://github.com/scryt/bips/blob/master/bip-0112.mediawiki))
- `scrytconsensus_SCRIPT_FLAGS_VERIFY_WITNESS` - Enable WITNESS ([BIP141](https://github.com/scryt/bips/blob/master/bip-0141.mediawiki))

##### Errors
- `scrytconsensus_ERR_OK` - No errors with input parameters *(see the return value of `scrytconsensus_verify_script` for the verification status)*
- `scrytconsensus_ERR_TX_INDEX` - An invalid index for `txTo`
- `scrytconsensus_ERR_TX_SIZE_MISMATCH` - `txToLen` did not match with the size of `txTo`
- `scrytconsensus_ERR_DESERIALIZE` - An error deserializing `txTo`
- `scrytconsensus_ERR_AMOUNT_REQUIRED` - Input amount is required if WITNESS is used

### Example Implementations
- [NScryt](https://github.com/NicolasDorier/NScryt/blob/master/NScryt/Script.cs#L814) (.NET Bindings)
- [node-libscrytconsensus](https://github.com/bitpay/node-libscrytconsensus) (Node.js Bindings)
- [java-libscrytconsensus](https://github.com/dexX7/java-libscrytconsensus) (Java Bindings)
- [scrytconsensus-php](https://github.com/Bit-Wasp/scrytconsensus-php) (PHP Bindings)
