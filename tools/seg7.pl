#!/usr/bin/perl

my @segments = qw(A B C D E F G DEC);

my @nums = ();

push @nums, [qw(A B C D E F)];

push @nums, [qw(B C)];

push @nums, [qw(A B D E G)];

push @nums, [qw(A B C D G)];

push @nums, [qw(F G B C)];

push @nums, [qw(A F G C D)];

push @nums, [qw(A F E D C G)];

push @nums, [qw(A B C)];

push @nums, [qw(A B C D E F G)];

push @nums, [qw(A B C D F G)];


my %seg_ports = (
                 "A" => 5,
                 "B" => 6,
                 "C" => 7,
                 "D" => 8,
                 "E" => 9,
                 "F" => 10,
                 "G" => 13,
                 "DEC" => 14,
               );

my %dig_ports = (
                 "1" => 0,
                 "2" => 1,
                 "3" => 2,
                 "4" => 3,
               );
           
#foreach my $segment(@segments){
           ##print "seg = $segment\n";
#}
print "const uint32_t seg_bsrr[] = {\n";
my $i = 0;
foreach my $num(@nums){
  my $mask = 0;
  foreach my $seg(@segments){

    if (grep { $_ eq $seg } @$num) {
      #print "$seg found at num $i\n";
      $mask |= (1<<$seg_ports{"$seg"});
    }
    else{
      $mask |= (1<<($seg_ports{"$seg"}+16));
      #print "$seg not found at num $i\n";
    }
  }
  printf "    ${mask}U";
  if ($i<9){
    print ",\n";
  }
  else{
    print "\n";
  }
  $i++;
}  
print "};\n\n";



print "const uint32_t dig_bsrr[] = {\n";
foreach my $digit (1.. 4){
  my $mask = 0;
  foreach my $i (0.. 3){
    if ($digit == ($i+1)) {
      $mask |= (1<<($dig_ports{"$digit"}+16));
      #my $temp = $dig_ports{"$digit"}+16;
      #print "debug: bit $temp is set to turn on digit $digit\n";
    }
    else{
      my $dig = $i+1;
      $mask |= (1<<$dig_ports{"$dig"});
      #my $temp = $dig_ports{"$dig"};
      #print "debug: bit $temp is set to turn off digit $dig\n";
    }
  }
  print "    ${mask}U";
  if ($digit<4){
    print ",\n";
  }
  else{
    print "\n";
  }
}
print "};\n\n";

my $mask = 0;
foreach my $i (0.. 3){
  my $digit = $i+1;
  $mask |= (1<<$dig_ports{"$digit"});
}
print "#define SEG7_BSRR_DIGS_OFF\t\t${mask}U\n\n";
