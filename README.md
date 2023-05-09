# AIS1002 project_template

> Starter kit for AIS1002 prosjektet. 

Dette er i bunn og grunn folderen `tests/threepp_vcpkg_test/vcpkg-configuration.json` dratt ut som eget repository.

### Oppdatere threepp

Oppdatering av threepp til nyere versjon skjer med å endre verdien av `baseline` vist nedenfor i [vcpkg-configuration.json](vcpkg-configuration.json).
```cpp
"registries": [
    {
      "kind": "git",
      "baseline": "9f301510467ed4efba09633bda84d9be69c5ebbc",
      "repository": "https://github.com/Ecos-platform/vcpkg-registry",
      "packages": [ "threepp" ]
    }
  ]
}
```

For å finne den til enhver tid nyeste tilgjengelige `baseline` sjekk tilsvarende fil i [threepp](https://github.com/markaren/threepp/blob/master/tests/threepp_vcpkg_test/vcpkg-configuration.json).

### Gjør prosjektet til deres eget.

Sett opp Git konfigurasjone mot deres eget reposity manuelt.
Legg til `.gitignore` og overskriv denne README.md i deres eget prosjekt.
