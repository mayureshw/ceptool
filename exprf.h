#define ETYPENUM { bool__, float__, int__, string__ }
#define LRET ExprBase*
#define LARG vector< ExprBase* >&  argv 
#define TARGMAP { { { "#", { int__ } }, [ this ] ( LARG ) -> LRET { return new CEPCount( this->_router, argv ); } }, { { "==", { int__, float__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int, float >( argv ); } }, { { "==", { int__, int__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< int, int >( argv ); } }, { { "==", { float__, int__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< float, int >( argv ); } }, { { "==", { float__, float__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< float, float >( argv ); } }, { { "==", { string__, string__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< string, string >( argv ); } }, { { "==", { bool__, bool__ } }, [  ] ( LARG ) -> LRET { return new CEPEq< bool, bool >( argv ); } }, { { "^", { bool__, bool__ } }, [  ] ( LARG ) -> LRET { return new CEPXor< bool, bool >( argv ); } } }
