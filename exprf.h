#ifndef _EXPRF_H
#define _EXPRF_H
typedef enum { bool__, double__, float__, int16_t__, int32_t__, int64_t__, int8_t__, int__, string__, uint16_t__, uint32_t__, uint64_t__, uint8_t__ } Etyp;
#define LRET ExprBase*
#define LARG vector< ExprBase* >&  argv 
#define TARGMAP { { { "*", { uint16_t__ } }, [ this ] ( LARG ) -> LRET { return new CEPReg< uint16_t >( this->_stateif, argv ); } }, { { "*", { uint32_t__ } }, [ this ] ( LARG ) -> LRET { return new CEPReg< uint32_t >( this->_stateif, argv ); } }, { { "*", { uint64_t__ } }, [ this ] ( LARG ) -> LRET { return new CEPReg< uint64_t >( this->_stateif, argv ); } }, { { "*", { int__ } }, [ this ] ( LARG ) -> LRET { return new CEPReg< int >( this->_stateif, argv ); } }, { { "*", { int8_t__ } }, [ this ] ( LARG ) -> LRET { return new CEPReg< int8_t >( this->_stateif, argv ); } }, { { "*", { int16_t__ } }, [ this ] ( LARG ) -> LRET { return new CEPReg< int16_t >( this->_stateif, argv ); } }, { { "*", { int32_t__ } }, [ this ] ( LARG ) -> LRET { return new CEPReg< int32_t >( this->_stateif, argv ); } }, { { "*", { int64_t__ } }, [ this ] ( LARG ) -> LRET { return new CEPReg< int64_t >( this->_stateif, argv ); } }, { { "*", { uint8_t__ } }, [ this ] ( LARG ) -> LRET { return new CEPReg< uint8_t >( this->_stateif, argv ); } }, { { "*", { double__ } }, [ this ] ( LARG ) -> LRET { return new CEPReg< double >( this->_stateif, argv ); } }, { { "*", { float__ } }, [ this ] ( LARG ) -> LRET { return new CEPReg< float >( this->_stateif, argv ); } }, { { "#", { int__ } }, [ this ] ( LARG ) -> LRET { return new CEPCount( this->_router, argv ); } }, { { "==", { uint16_t__, uint16_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint16_t, uint16_t >( argv ); } }, { { "==", { uint16_t__, uint32_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint16_t, uint32_t >( argv ); } }, { { "==", { uint16_t__, uint64_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint16_t, uint64_t >( argv ); } }, { { "==", { uint16_t__, int__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint16_t, int >( argv ); } }, { { "==", { uint16_t__, int8_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint16_t, int8_t >( argv ); } }, { { "==", { uint16_t__, int16_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint16_t, int16_t >( argv ); } }, { { "==", { uint16_t__, int32_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint16_t, int32_t >( argv ); } }, { { "==", { uint16_t__, int64_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint16_t, int64_t >( argv ); } }, { { "==", { uint16_t__, uint8_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint16_t, uint8_t >( argv ); } }, { { "==", { uint16_t__, double__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint16_t, double >( argv ); } }, { { "==", { uint16_t__, float__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint16_t, float >( argv ); } }, { { "==", { uint32_t__, uint16_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint32_t, uint16_t >( argv ); } }, { { "==", { uint32_t__, uint32_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint32_t, uint32_t >( argv ); } }, { { "==", { uint32_t__, uint64_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint32_t, uint64_t >( argv ); } }, { { "==", { uint32_t__, int__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint32_t, int >( argv ); } }, { { "==", { uint32_t__, int8_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint32_t, int8_t >( argv ); } }, { { "==", { uint32_t__, int16_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint32_t, int16_t >( argv ); } }, { { "==", { uint32_t__, int32_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint32_t, int32_t >( argv ); } }, { { "==", { uint32_t__, int64_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint32_t, int64_t >( argv ); } }, { { "==", { uint32_t__, uint8_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint32_t, uint8_t >( argv ); } }, { { "==", { uint32_t__, double__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint32_t, double >( argv ); } }, { { "==", { uint32_t__, float__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint32_t, float >( argv ); } }, { { "==", { uint64_t__, uint16_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint64_t, uint16_t >( argv ); } }, { { "==", { uint64_t__, uint32_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint64_t, uint32_t >( argv ); } }, { { "==", { uint64_t__, uint64_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint64_t, uint64_t >( argv ); } }, { { "==", { uint64_t__, int__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint64_t, int >( argv ); } }, { { "==", { uint64_t__, int8_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint64_t, int8_t >( argv ); } }, { { "==", { uint64_t__, int16_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint64_t, int16_t >( argv ); } }, { { "==", { uint64_t__, int32_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint64_t, int32_t >( argv ); } }, { { "==", { uint64_t__, int64_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint64_t, int64_t >( argv ); } }, { { "==", { uint64_t__, uint8_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint64_t, uint8_t >( argv ); } }, { { "==", { uint64_t__, double__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint64_t, double >( argv ); } }, { { "==", { uint64_t__, float__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint64_t, float >( argv ); } }, { { "==", { bool__, bool__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< bool, bool >( argv ); } }, { { "==", { int__, uint16_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int, uint16_t >( argv ); } }, { { "==", { int__, uint32_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int, uint32_t >( argv ); } }, { { "==", { int__, uint64_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int, uint64_t >( argv ); } }, { { "==", { int__, int__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int, int >( argv ); } }, { { "==", { int__, int8_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int, int8_t >( argv ); } }, { { "==", { int__, int16_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int, int16_t >( argv ); } }, { { "==", { int__, int32_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int, int32_t >( argv ); } }, { { "==", { int__, int64_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int, int64_t >( argv ); } }, { { "==", { int__, uint8_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int, uint8_t >( argv ); } }, { { "==", { int__, double__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int, double >( argv ); } }, { { "==", { int__, float__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int, float >( argv ); } }, { { "==", { int8_t__, uint16_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int8_t, uint16_t >( argv ); } }, { { "==", { int8_t__, uint32_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int8_t, uint32_t >( argv ); } }, { { "==", { int8_t__, uint64_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int8_t, uint64_t >( argv ); } }, { { "==", { int8_t__, int__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int8_t, int >( argv ); } }, { { "==", { int8_t__, int8_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int8_t, int8_t >( argv ); } }, { { "==", { int8_t__, int16_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int8_t, int16_t >( argv ); } }, { { "==", { int8_t__, int32_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int8_t, int32_t >( argv ); } }, { { "==", { int8_t__, int64_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int8_t, int64_t >( argv ); } }, { { "==", { int8_t__, uint8_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int8_t, uint8_t >( argv ); } }, { { "==", { int8_t__, double__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int8_t, double >( argv ); } }, { { "==", { int8_t__, float__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int8_t, float >( argv ); } }, { { "==", { int16_t__, uint16_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int16_t, uint16_t >( argv ); } }, { { "==", { int16_t__, uint32_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int16_t, uint32_t >( argv ); } }, { { "==", { int16_t__, uint64_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int16_t, uint64_t >( argv ); } }, { { "==", { int16_t__, int__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int16_t, int >( argv ); } }, { { "==", { int16_t__, int8_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int16_t, int8_t >( argv ); } }, { { "==", { int16_t__, int16_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int16_t, int16_t >( argv ); } }, { { "==", { int16_t__, int32_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int16_t, int32_t >( argv ); } }, { { "==", { int16_t__, int64_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int16_t, int64_t >( argv ); } }, { { "==", { int16_t__, uint8_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int16_t, uint8_t >( argv ); } }, { { "==", { int16_t__, double__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int16_t, double >( argv ); } }, { { "==", { int16_t__, float__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int16_t, float >( argv ); } }, { { "==", { int32_t__, uint16_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int32_t, uint16_t >( argv ); } }, { { "==", { int32_t__, uint32_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int32_t, uint32_t >( argv ); } }, { { "==", { int32_t__, uint64_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int32_t, uint64_t >( argv ); } }, { { "==", { int32_t__, int__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int32_t, int >( argv ); } }, { { "==", { int32_t__, int8_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int32_t, int8_t >( argv ); } }, { { "==", { int32_t__, int16_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int32_t, int16_t >( argv ); } }, { { "==", { int32_t__, int32_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int32_t, int32_t >( argv ); } }, { { "==", { int32_t__, int64_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int32_t, int64_t >( argv ); } }, { { "==", { int32_t__, uint8_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int32_t, uint8_t >( argv ); } }, { { "==", { int32_t__, double__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int32_t, double >( argv ); } }, { { "==", { int32_t__, float__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int32_t, float >( argv ); } }, { { "==", { int64_t__, uint16_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int64_t, uint16_t >( argv ); } }, { { "==", { int64_t__, uint32_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int64_t, uint32_t >( argv ); } }, { { "==", { int64_t__, uint64_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int64_t, uint64_t >( argv ); } }, { { "==", { int64_t__, int__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int64_t, int >( argv ); } }, { { "==", { int64_t__, int8_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int64_t, int8_t >( argv ); } }, { { "==", { int64_t__, int16_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int64_t, int16_t >( argv ); } }, { { "==", { int64_t__, int32_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int64_t, int32_t >( argv ); } }, { { "==", { int64_t__, int64_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int64_t, int64_t >( argv ); } }, { { "==", { int64_t__, uint8_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int64_t, uint8_t >( argv ); } }, { { "==", { int64_t__, double__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int64_t, double >( argv ); } }, { { "==", { int64_t__, float__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int64_t, float >( argv ); } }, { { "==", { uint8_t__, uint16_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint8_t, uint16_t >( argv ); } }, { { "==", { uint8_t__, uint32_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint8_t, uint32_t >( argv ); } }, { { "==", { uint8_t__, uint64_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint8_t, uint64_t >( argv ); } }, { { "==", { uint8_t__, int__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint8_t, int >( argv ); } }, { { "==", { uint8_t__, int8_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint8_t, int8_t >( argv ); } }, { { "==", { uint8_t__, int16_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint8_t, int16_t >( argv ); } }, { { "==", { uint8_t__, int32_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint8_t, int32_t >( argv ); } }, { { "==", { uint8_t__, int64_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint8_t, int64_t >( argv ); } }, { { "==", { uint8_t__, uint8_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint8_t, uint8_t >( argv ); } }, { { "==", { uint8_t__, double__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint8_t, double >( argv ); } }, { { "==", { uint8_t__, float__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< uint8_t, float >( argv ); } }, { { "==", { double__, uint16_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< double, uint16_t >( argv ); } }, { { "==", { double__, uint32_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< double, uint32_t >( argv ); } }, { { "==", { double__, uint64_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< double, uint64_t >( argv ); } }, { { "==", { double__, int__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< double, int >( argv ); } }, { { "==", { double__, int8_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< double, int8_t >( argv ); } }, { { "==", { double__, int16_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< double, int16_t >( argv ); } }, { { "==", { double__, int32_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< double, int32_t >( argv ); } }, { { "==", { double__, int64_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< double, int64_t >( argv ); } }, { { "==", { double__, uint8_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< double, uint8_t >( argv ); } }, { { "==", { double__, double__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< double, double >( argv ); } }, { { "==", { double__, float__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< double, float >( argv ); } }, { { "==", { float__, uint16_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< float, uint16_t >( argv ); } }, { { "==", { float__, uint32_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< float, uint32_t >( argv ); } }, { { "==", { float__, uint64_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< float, uint64_t >( argv ); } }, { { "==", { float__, int__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< float, int >( argv ); } }, { { "==", { float__, int8_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< float, int8_t >( argv ); } }, { { "==", { float__, int16_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< float, int16_t >( argv ); } }, { { "==", { float__, int32_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< float, int32_t >( argv ); } }, { { "==", { float__, int64_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< float, int64_t >( argv ); } }, { { "==", { float__, uint8_t__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< float, uint8_t >( argv ); } }, { { "==", { float__, double__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< float, double >( argv ); } }, { { "==", { float__, float__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< float, float >( argv ); } }, { { "==", { string__, string__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< string, string >( argv ); } }, { { "^", { bool__, bool__ } }, [  ] ( LARG ) -> LRET { return new CEPXor< bool, bool >( argv ); } } }
#endif
