#include <stdio.h>

#include "eav.h"

int try_rfc_6531() {
  eav_t eav;
  eav_init(&eav);
  eav.tld_check = false;
  eav.rfc = EAV_RFC_6531;
  eav_setup(&eav);
  const char eml[] = "abc@def_ghi.us";
  int len = sizeof(eml) - 1;
  int ret = eav_is_email(&eav, eml, len);
  eav_free(&eav);
  return ret;
}

int try_rfc_5321() {
  eav_t eav;
  eav_init(&eav);
  eav.tld_check = false;
  eav.rfc = EAV_RFC_5321;
  eav_setup(&eav);
  const char eml[] = "abc@def_ghi.us";
  int len = sizeof(eml) - 1;
  int ret = eav_is_email(&eav, eml, len);
  eav_free(&eav);
  return ret;
}

int main() {
    printf("rfc-6531 for underscore=%d\nrfc-5321 for underscore=%d",
    try_rfc_6531(), try_rfc_5321());
    return 0;
}
