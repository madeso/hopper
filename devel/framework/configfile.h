
/** @brief manages a configuration file.
*	usage : load(), then : getValue() or setValue(), save() ( see the usage example at the end of this file )
*	saving only change values : the lines are not reordered, comments stay where they are
*	sections (i.e.[xxxxxx] ) are ignored ( as any line without a '=' )
*	comments supported anywhere ( comment token : ';' )
*
*	\bbeware :
*	ascii ony
*	keys are unique, if multiple keys, only the first one is used
*	no exceptions thrown if an error occured ( though this should be easy to add )
*	if multiple '=' in a line, only the first one is used. ( this means the value will contain one or more '=' )
*	only basic types are supported ( long, int, float, string, etc... )
*/



#ifndef _CCONFIGFILE_
#define _CCONFIGFILE_

#include <iostream>
#include <string>
#include <sstream>
#include <istream>
#include <fstream>
#include <map>

class CConfigFile {

	public:
	CConfigFile( std::string aConfigFile ) {
	    	mConfigFile=aConfigFile;
	}

	~CConfigFile() {
	    mOptions.clear();
	}

	bool load() {
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



	bool save() {
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




	bool getValue( const std::string& name, std::string& value ) {
    	std::map< std::string, std::string>::const_iterator it;
        it = mOptions.find(	name );
        if( it != mOptions.end() ) {
            value=(*it).second;
            return(true);
        }
        return( false);
	}


	template< typename T > bool getValue( const std::string& name, T& value ) {
    	std::stringstream ss;
        std::string s;

        if( getValue( name, s )) {
            ss << s;
            ss >> static_cast<T&>(value);
            return(true);
        }
        return(false);
	}


	bool setValue( const std::string& name, const std::string& value ) {
        std::map< std::string, std::string>::iterator it;
        it = mOptions.find(	name );
        if( it != mOptions.end() ) {
            (*it).second=value;
            return(true);
        }
        return(false);
	}

	template< typename T > bool setValue( const std::string& name, const T value ) {
        std::stringstream ss;
        ss << static_cast<T>(value);

        return( setValue( name, ss.str() ) );
	}


	void print() {

        std::map< std::string, std::string >::const_iterator it;

        int i=0;
        for( it = mOptions.begin(); it != mOptions.end(); ++it ) {
            std::cout << i << ":\"" << (*it).first << "\" = \"" << (*it).second << "\"" << std::endl;
            i++;
        }
	}


	private:

	void trim( std::string& str ) {

    	std::size_t pos;

        while(  (*str.begin()) == ' ' ) {
            str.erase( str.begin() );
        }
        while( (*str.rbegin()) == ' ' ) {
            str.erase( str.end()-1 );
        }

	}


	bool extractOptionValue( std::string line, std::string& option, std::string& value, std::string& comment ) {

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


	std::map< std::string, std::string > mOptions;
	std::string mConfigFile;

};

/*

class CConfigFile {

	public:
	CConfigFile( std::string aConfigFile );
	~CConfigFile();

	bool load();
	bool save();

	bool getValue( const std::string& name, std::string& value );
	template< typename T > bool getValue( const std::string& name, T& value );


	bool setValue( const std::string& name, const std::string& value );
	template< typename T > bool setValue( const std::string& name, const T& value );


	void print();

	private:
	void trim( std::string& str );
	bool extractOptionValue( std::string line, std::string& option, std::string& value, std::string& comment );
	std::map< std::string, std::string > mOptions;
	std::string mConfigFile;

};

*/

#endif




#if 0

// ============ Usage example ============ :

int main( int argc, char* argv[] ) {

	if( argc != 2 ) {
		std::cout << "Usage : " << argv[0] << " <config filename>" << std::endl;
		return(1);
	}

	CConfigFile conf( argv[1] );

	if( conf.load() ) {

		int i;
		if( conf.getValue("inputconfig2.pitch.max", i ) ) conf.setValue( "inputconfig2.pitch.max", 255 );
		if( conf.getValue("inputconfig2.pitch.min", i ) ) conf.setValue( "inputconfig2.pitch.min", -255 );

		conf.save();
	}

	return(0);
}

#endif


