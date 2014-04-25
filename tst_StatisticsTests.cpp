#include <QString>
#include <QtTest>
#include <stdexcept>
#include <sstream>

#include <vector>

enum ExceptionType {
    noException,
    emptyArgument,
    outOfRange
};

class LogicError : public std::logic_error {
public:

    LogicError( ) : std::logic_error( "" ) {

    }

    virtual const char *what( ) const throw () {
        return m_message.c_str( );
    }

    virtual ~LogicError( ) throw () {

    }

protected:
    std::string m_message;
};

class OutOfRange : public LogicError {
public:

    OutOfRange( int argument, int beginOfRange, int endOfRange ) {
        std::string str_argument, str_beginOfRange, str_endOfRange;

        str_argument = intToString( argument );
        str_beginOfRange = intToString( beginOfRange );
        str_endOfRange = intToString( endOfRange );

        m_message = "Argument " + str_argument + " doesn't hit in the range [" +
                str_beginOfRange + ", " + str_endOfRange + "]";
    }

private:

    std::string intToString( int number ) {
        std::stringstream stream;
        stream << number;
        return stream.str( );
    }
};

class EmptyArgument : public LogicError {
public:

    EmptyArgument( ) {
        m_message = "Error: empty argument.";
    }
};

bool statistics( const std::vector<int> &inputArray, std::vector<int> &oddArray, std::vector<int> &evenArray ) throw (EmptyArgument, OutOfRange);

class StatisticsTests : public QObject {
    Q_OBJECT

public:
    StatisticsTests( );

    private
Q_SLOTS:
    void testCase1_data( );
    void testCase1( );
};

StatisticsTests::StatisticsTests( ) {
}

void StatisticsTests::testCase1_data( ) {
    QTest::addColumn< std::vector<int> >("inputArray");
    QTest::addColumn< std::vector<int> >("expectedOdd");
    QTest::addColumn< std::vector<int> >("expectedEven");
    QTest::addColumn< bool >("expectedIsYes");
    QTest::addColumn< int >("exceptionType");

    std::vector<int> inputArray;
    std::vector<int> expectedOdd;
    std::vector<int> expectedEven;
    bool expectedIsYes;
    ExceptionType exceptionType;

    inputArray.push_back( 4 );
    inputArray.push_back( 16 );
    inputArray.push_back( 19 );
    inputArray.push_back( 31 );
    inputArray.push_back( 2 );

    expectedOdd.push_back( 19 );
    expectedOdd.push_back( 31 );

    expectedEven.push_back( 4 );
    expectedEven.push_back( 16 );
    expectedEven.push_back( 2 );

    expectedIsYes = true;

    exceptionType = noException;

    QTest::newRow( "Normal: Yes" ) << inputArray << expectedOdd << expectedEven << expectedIsYes << (int) exceptionType;
    inputArray.clear( );
    expectedOdd.clear( );
    expectedEven.clear( );

    // Normal: No
    inputArray.push_back( 29 );
    inputArray.push_back( 4 );
    inputArray.push_back( 7 );
    inputArray.push_back( 12 );
    inputArray.push_back( 15 );
    inputArray.push_back( 17 );
    inputArray.push_back( 24 );
    inputArray.push_back( 1 );

    expectedOdd.push_back( 29 );
    expectedOdd.push_back( 7 );
    expectedOdd.push_back( 15 );
    expectedOdd.push_back( 17 );
    expectedOdd.push_back( 1 );

    expectedEven.push_back( 4 );
    expectedEven.push_back( 12 );
    expectedEven.push_back( 24 );

    expectedIsYes = false;

    exceptionType = noException;

    QTest::newRow( "Normal: No" ) << inputArray << expectedOdd << expectedEven << expectedIsYes << (int) exceptionType;
    inputArray.clear( );
    expectedOdd.clear( );
    expectedEven.clear( );

    // Normal: The Same Size
    inputArray.push_back( 29 );
    inputArray.push_back( 4 );
    inputArray.push_back( 7 );
    inputArray.push_back( 12 );
    inputArray.push_back( 24 );
    inputArray.push_back( 1 );

    expectedOdd.push_back( 29 );
    expectedOdd.push_back( 7 );
    expectedOdd.push_back( 1 );

    expectedEven.push_back( 4 );
    expectedEven.push_back( 12 );
    expectedEven.push_back( 24 );

    expectedIsYes = true;

    exceptionType = noException;

    QTest::newRow( "Normal: The Same Size" ) << inputArray << expectedOdd << expectedEven << expectedIsYes << (int) exceptionType;
    inputArray.clear( );
    expectedOdd.clear( );
    expectedEven.clear( );

    // Empty Argument
    exceptionType = emptyArgument;
    QTest::newRow( "Empty Argument" ) << inputArray << expectedOdd << expectedEven << expectedIsYes << (int) exceptionType;

    // Out Of Range
    inputArray.push_back( 4 );
    inputArray.push_back( 0 );
    inputArray.push_back( 19 );
    inputArray.push_back( 31 );
    inputArray.push_back( 2 );
    exceptionType = outOfRange;
    QTest::newRow( "Out Of Range" ) << inputArray << expectedOdd << expectedEven << expectedIsYes << (int) exceptionType;
    inputArray.clear( );

    // Out Of Range
    inputArray.push_back( 4 );
    inputArray.push_back( 32 );
    inputArray.push_back( 19 );
    inputArray.push_back( 31 );
    inputArray.push_back( 2 );
    exceptionType = outOfRange;
    QTest::newRow( "Out Of Range" ) << inputArray << expectedOdd << expectedEven << expectedIsYes << (int) exceptionType;
    inputArray.clear( );
}

void StatisticsTests::testCase1( ) {
    QFETCH( std::vector<int>, inputArray );
    QFETCH( std::vector<int>, expectedOdd );
    QFETCH( std::vector<int>, expectedEven );
    QFETCH( bool, expectedIsYes );
    QFETCH( int, exceptionType );

    bool isYes = false;
    std::vector<int> oddArray;
    std::vector<int> evenArray;

    try {
        isYes = statistics( inputArray, oddArray, evenArray );
        if ( exceptionType != noException ) {
            QVERIFY2( false, "There is no exception." );
        } else {
            if ( isYes == expectedIsYes ) {
                QVERIFY2( true, "" );
            } else {
                QVERIFY2( false, "isYes != expectedIsYes" );
            }

            // Check sizes
            if ( expectedOdd.size( ) == oddArray.size( ) ) {
                QVERIFY2( true, "" );
            } else {
                QVERIFY2( false, "expectedOdd.size() != oddArray.size()" );
            }
            if ( expectedEven.size( ) == evenArray.size( ) ) {
                QVERIFY2( true, "" );
            } else {
                QVERIFY2( false, "expectedEven.size() != evenArray.size()" );
            }

            // Check odd values
            for ( std::size_t i = 0; i < oddArray.size( ); ++i ) {
                if ( oddArray[i] == expectedOdd[i] ) {
                    QVERIFY2( true, "" );
                } else {
                    std::ostringstream ostr;
                    ostr << "oddArray[" << i << "] == expectedOdd[" << i << "]";
                    QVERIFY2( false, ostr.str( ).c_str( ) );
                }
            }

            // Check even values
            for ( std::size_t i = 0; i < evenArray.size( ); ++i ) {
                if ( evenArray[i] == expectedEven[i] ) {
                    QVERIFY2( true, "" );
                } else {
                    std::ostringstream ostr;
                    ostr << "evenArray[" << i << "] == expectedEven[" << i << "]";
                    QVERIFY2( false, ostr.str( ).c_str( ) );
                }
            }
        }
    } catch ( const LogicError& e ) {
        switch ( exceptionType ) {
            case noException:
                QVERIFY2( false, "Exception was occur." );
                break;
            case emptyArgument:
                if ( std::string( "Error: empty argument." ) == std::string( e.what( ) ) ) {
                    QVERIFY2( true, "" );
                } else {
                    QVERIFY2( false, "We expecte EmptyArgument exception." );
                }
                break;
            case outOfRange:
                if ( std::string( "Argument 0 doesn't hit in the range [1, 31]" ) == std::string( e.what( ) ) ||
                        std::string( "Argument 32 doesn't hit in the range [1, 31]" ) == std::string( e.what( ) ) ) {
                    QVERIFY2( true, "" );
                } else {
                    QVERIFY2( false, "We expecte OutOfRange exception." );
                }
                break;
            default:
                QVERIFY2( false, "We don't expecte this exception." );
                break;
        }
    } catch ( ... ) {
        QVERIFY2( false, "Uncaught exception." );
    }
}

QTEST_APPLESS_MAIN( StatisticsTests )

#include "tst_StatisticsTests.moc"
