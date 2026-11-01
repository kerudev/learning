import 'package:flutter/material.dart';

import 'package:birdle/root.dart';
import 'package:birdle/utils/context.dart';

void main() async {
  await initContext();

  runApp(const Root());
}
