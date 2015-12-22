
#include "configfile.h"


CConfigFile::CConfigFile( std::string aConfigFile ) {
	mConfigFile=aConfigFile;
}

CConfigFile::~CConfigFile() {
	mOptions.clear();
}

bool CConfigFile::load() {

	std::ifstream fp;
	fp.open( mConfigFile.c_str(), std::ios_base::in );

	if( !fp ) {
		std::cout << "Error : can't open \"" << mConfigFile << "\" !" << std::endl;
		return(false);
	}

	std::string line;
	std::string option;
	std::string value;
	std::string comment;
	std::size_t pos;

	while( !fp.eof() ) {
		std::getline( fp, line);

		if( extractOptionValue( line, option, value, comment ) ) {
			if( option.size()>0 && value.size()>0 ) {
				mOptions.insert( std::pair< std::string, std::string>( option, value )) ;
			}
		}

	}

	fp.close();

	return(true);
}


void CConfigFile::trim( std::string& str ) {
	std::size_t pos;

	while(  (*str.begin()) == ' ' ) {
		str.erase( str.begin() );
	}
	while( (*str.rbegin()) == ' ' ) {
		str.erase( str.end()-1 );
	}

}


void CConfigFile::print() {

	std::map< std::string, std::string >::const_iterator it;

	int i=0;
	for( it = mOptions.begin(); it != mOptions.end(); ++it ) {
		std::cout << i << ":\"" << (*it).first << "\" = \"" << (*it).second << "\"" << std::endl;
		i++;
	}

}

template< typename T> bool CConfigFile::getValue( const std::string& name, T& value ) {
	std::stringstream ss;
	std::string s;

	if( getValue( name, s )) {
		ss << s;
		ss >> static_cast<T&>(value);
		return(true);
	}
	return(false);
}


/*
bool CConfigFile::getValue( const std::string& name, std::string& value ) {
	std::map< std::string, std::string>::const_iterator it;
	it = mOptions.find(	name );
	if( it != mOptions.end() ) {
		value=(*it).second;
		return(true);
	}
	return( false);
}
*/


bool CConfigFile::setValue( const std::string& name, const std::string& value ) {
	std::map< std::string, std::string>::iterator it;
	it = mOptions.find(	name );
	if( it != mOptions.end() ) {
		(*it).second=value;
		return(true);
	}
	return(false);
}

template< typename T>
bool CConfigFile::setValue( const std::string& name, const T& value ) {
	std::stringstream ss;
	ss << static_cast<T&>(value);

	return( setValue( name, ss.str() ) );

}

/**
	returns true and option and value and comment if option and value found
    returns false otherwise ( comment in comment if found )
*/
bool CConfigFile::extractOptionValue( std::string line, std::string& option, std::string& value, std::string& comment ) {

	std::size_t pos;

	pos=line.find_first_of( ';' );   // manage comments
	if( pos != std::string::npos ) {
		comment=line.substr( pos, line.size() );
		line.erase( pos );
		trim( line );
	}

	if( !line.empty() ) {
		pos=line.find_first_of( '=' );
		if( pos == std::string::npos ) return( false ); // ignore lines with no '='
		option=line.substr( 0, pos );
		value=line.substr( pos+1, line.size() );
		trim( option );
		trim( value );
	}
	else return( false );

	return( true );

}


bool CConfigFile::save() {

	std::ofstream ofp;
	std::ifstream ifp;
	std::string filename( mConfigFile+".tmp" );

	ofp.open( filename.c_str(), std::ios_base::trunc );

	if( !ofp ) {
		std::cout << "Error : can't open output file \"" << mConfigFile << "\" !"  << std::endl;
		return(false);
	}

	ifp.open( mConfigFile.c_str(), std::ios_base::in );

	if( !ifp ) {
		std::cout << "Error : can't open input file \"" << mConfigFile << "\" !" << std::endl;
		return(false);
	}

	std::string line;
	std::string option;
	std::string value;
	std::string comment;

	std::map< std::string, std::string >::const_iterator it;

	while( !ifp.eof() ) {

		std::getline( ifp, line);

		comment.clear();

		if( extractOptionValue( line, option, value, comment ) ) {
			it = mOptions.find(	option );
			if( it != mOptions.end() ) {
				value=(*it).second;
			}
			ofp << option << " = " << value << " " << comment << std::endl;
		}
		else {
			ofp << line << std::endl;
		}

	}

	ifp.close();

	ofp.flush();
	ofp.close();

	if ( remove( mConfigFile.c_str() ) ) {
		std::cout << "Error : can't remove \"" <<  mConfigFile << "\" !";
		perror("( " );
		std::cout << " )" << std::endl;
		return( false );
	}
	if ( rename( filename.c_str(), mConfigFile.c_str() ) ) {
		std::cout << "Error : can't rename \"" << filename << "\"" << " to \"" << mConfigFile << "\" !";
		perror("(" );
		std::cout << ")" << std::endl;
		return( false );
	}

	return(true);

}



