// ==========================================================================
// Dedmonwakeen's Raid DPS/TPS Simulator.
// Send questions to natehieter@gmail.com
// ==========================================================================
#ifndef SC_DBC_CLIENT_HOTFIX_ENTRY_HPP
#define SC_DBC_CLIENT_HOTFIX_ENTRY_HPP

#include "util/span.hpp"

#include <cstdint>

namespace hotfix
{
// WoW Client hotfixing entry data type. Note that unlike most hotfix namespace methods, the type
// is declared here (instead of dbc.hpp), since the hotfix data tables are included in
// sc_spell_data.inc. Hotfix data is generated by the dbc_extract tool simultaneously with spell data.

// Type of the hotfixing entry, support for signed, unsigned, floats (double) and strings
// (c-strings), since we only use this information for output purposes. Actual correct data-type
// specific values are already in the static data.
enum dbc_ftype { UINT = 0U, INT, FLOAT, STRING };

// A hotfix data entry representing the original value of the data before it was hotfixed
struct client_hotfix_entry_t
{
  unsigned  id;                // Record ID (e.g., spell, effect, or power id)
  unsigned  field_id;          // Internal field ID (the field number inside the data struct)
  dbc_ftype field_type;        // Internal field type

  // The original value (hotfix value is in the actual data). Union type based on field_type.
  union value_t
  {
    uint32_t    u;
    int32_t     i;
    double      f;
    const char* s;

    constexpr value_t( uint32_t v )    : u( v ) {}
    constexpr value_t( int32_t v )     : i( v ) {}
    constexpr value_t( double v )      : f( v ) {}
    constexpr value_t( const char* v ) : s( v ) {}
  } orig_value, hotfixed_value;

  // Required for msvc to create 0-size std::arrays
  constexpr client_hotfix_entry_t() :
    id( 0 ), field_id( -1 ), field_type( INT ), orig_value( 0 ), hotfixed_value( 0 )
  { }

  constexpr client_hotfix_entry_t( unsigned id, unsigned field, const char* s, const char* hotfix_s ) :
    id( id ), field_id( field ), field_type( STRING ), orig_value( s ), hotfixed_value( hotfix_s )
  { }

  constexpr client_hotfix_entry_t( unsigned id, unsigned field, int32_t i, int32_t hotfix_i ) :
    id( id ), field_id( field ), field_type( INT ), orig_value( i ), hotfixed_value( hotfix_i )
  { }

  constexpr client_hotfix_entry_t( unsigned id, unsigned field, uint32_t u, uint32_t hotfix_u ) :
    id( id ), field_id( field ), field_type( UINT ), orig_value( u ), hotfixed_value( hotfix_u )
  { }

  constexpr client_hotfix_entry_t( unsigned id, unsigned field, double f, double hotfix_f ) :
    id( id ), field_id( field ), field_type( FLOAT ), orig_value( f ), hotfixed_value( hotfix_f )
  { }
};

// Returns hotfix entries for an object of a specified type
util::span<const client_hotfix_entry_t> spell_hotfixes( unsigned id, bool ptr );
util::span<const client_hotfix_entry_t> effect_hotfixes( unsigned id, bool ptr );
util::span<const client_hotfix_entry_t> power_hotfixes( unsigned id, bool ptr );

} // namespace hotfix

#endif // SC_DBC_CLIENT_HOTFIX_ENTRY_HPP
